// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/StelaeKnight.h"
#include "Components/CapsuleComponent.h"
#include "AI/BossMonsterController.h"
#include "CharacterStat/CharacterStatComponent.h"
#include "Character/Enemy/MonsterSpawner.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/BossMonsterAnimInstance.h"
#include "Engine/DamageEvents.h"
#include "Components/WidgetComponent.h"

AStelaeKnight::AStelaeKnight()
{
	AIControllerClass = ABossMonsterController::StaticClass();

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SKM(TEXT("/Script/Engine.SkeletalMesh'/Game/_ART/Character/StelaeKnight/StelaeKnight.StelaeKnight'"));
	if (SKM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SKM.Object);
	}
	static ConstructorHelpers::FClassFinder<UAnimInstance>
		AnimClass(TEXT("/Game/_Programming/Character/Enemy/StelaeKnight/ABP_StelaeKnight.ABP_StelaeKnight_C"));
	if (AnimClass.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimClass.Class);
	}
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -120.f));

	GetCapsuleComponent()->SetCapsuleHalfHeight(120.f);

	mSword = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sword"));
	mSword->SetupAttachment(GetMesh(), TEXT("Sword"));
	mSword->SetCanEverAffectNavigation(false);
	mSword->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		SM(TEXT("/Script/Engine.StaticMesh'/Game/_ART/Character/UndeadPack/SkeletonEnemy/Mesh/Weapon/Sword/SM_Sword.SM_Sword'"));
	if (SM.Succeeded())
	{
		mSword->SetStaticMesh(SM.Object);
	}
	
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->MaxWalkSpeed = 200.f;
	
	bIsBoss = true;

	mCollisionStartPos = CreateDefaultSubobject<USceneComponent>(TEXT("CollisionStartPos"));
	mCollisionStartPos->SetupAttachment(mSword);

	mCollisionEndPos = CreateDefaultSubobject<USceneComponent>(TEXT("CollisionEndPos"));
	mCollisionEndPos->SetupAttachment(mSword);

	mCollisionRadius = CreateDefaultSubobject<USceneComponent>(TEXT("CollisionRadius"));
	mCollisionRadius->SetupAttachment(mSword);


	mHpBarWidget->SetDrawSize(FVector2D(300.f, 15.f));
	mHpBarWidget->SetRelativeLocation(FVector(0.f, 0.f, 150.f));
}

void AStelaeKnight::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	mStats->SetStats(5);
	SetAttackRange(300.f);
}

void AStelaeKnight::BeginPlay()
{
	Super::BeginPlay();

}

void AStelaeKnight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	FVector VecToTarget = mTarget->GetActorLocation() - GetActorLocation();
	VecToTarget.Z = 0;
	SetActorRotation(FMath::RInterpTo(GetActorRotation(), VecToTarget.Rotation(), DeltaTime, 5.f));
}

float AStelaeKnight::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);



	return Damage;
}

void AStelaeKnight::GetHit_Implementation(const FVector& ImpactPoint)
{
	Super::GetHit_Implementation(ImpactPoint);

	FVector ForwardVec = GetActorForwardVector();
	FVector VecToImpactPoint = ImpactPoint - GetActorLocation();

	// 좌 우 판단
	FVector CrossRes = FVector::CrossProduct(ForwardVec, VecToImpactPoint);

	Cast<UBossMonsterAnimInstance>(mAnimInstance)->PlayMontage(TEXT("Hit"),
		(CrossRes.Z > 0) ? TEXT("R") : TEXT("L"));
}

void AStelaeKnight::AttackCollisionCheck(EAttackType AttackType)
{
	Super::AttackCollisionCheck(AttackType);

	// 이전 프레임 검 중앙 위치
	FVector Start = mPrevCollisionPos;
	// 현재 프레임 검 중앙 위치
	FVector End = (GetCollisionStartPos() + GetCollisionEndPos()) * 0.5f;

	if (Start == End) return;

	float Radius = (GetCollisionStartPos() - GetCollisionRadiusPos()).Length();
	FVector Origin = (Start + End) * 0.5f;

	FCollisionQueryParams Params(NAME_None, false, this);
	TArray<FHitResult> HitResults;
	bool Collision = GetWorld()->SweepMultiByChannel(OUT HitResults,
		Start, End, FRotationMatrix::MakeFromXZ(GetActorForwardVector(), (End - Start)).ToQuat(),
		ECollisionChannel::ECC_GameTraceChannel3,
		FCollisionShape::MakeBox(
			FVector((GetCollisionEndPos() - GetCollisionStartPos()).Length() * 0.5f,
				Radius, Radius)), Params);

	if (Collision)
	{
		for (const FHitResult& HitResult : HitResults)
		{
			IHitInterface* HitInterface = Cast<IHitInterface>(HitResult.GetActor());
			if (HitInterface)
			{
				if (HitInterface->IsDamaged())	continue;
				HitInterface->SetDamaged(true);
				mHitInterface = HitInterface;

				FDamageEvent DmgEvent;
				HitResult.GetActor()->TakeDamage(mStats->GetAtk(), DmgEvent, GetController(), this);

				HitInterface->Execute_GetHit(HitResult.GetActor(), GetActorLocation());
			}
		}
	}


#if ENABLE_DRAW_DEBUG
	if (bDrawDebug)
	{
		FColor DrawColor = Collision ? FColor::Green : FColor::Red;

		DrawDebugBox(GetWorld(), Origin,
			FVector((GetCollisionEndPos() - GetCollisionStartPos()).Length() * 0.5,
				Radius, (Origin - Start).Length() + Radius),
			FRotationMatrix::MakeFromXZ(GetActorForwardVector(), (End - Start)).ToQuat(),
			DrawColor, false, 1.f);
	}
#endif
	// 다음 프레임을 위해 현재 프레임 검 중앙 위치 저장
	mPrevCollisionPos = (GetCollisionStartPos() + GetCollisionEndPos()) * 0.5f;
}

void AStelaeKnight::Attack()
{
	if (!IsValid(mTarget)) return;
	if (!IsValid(mAnimInstance))	return;

	UBossMonsterAnimInstance* AnimInstance = Cast<UBossMonsterAnimInstance>(mAnimInstance);
	if (nullptr == AnimInstance)	return;
	
	float DistToTarget = FVector::Dist(mTarget->GetActorLocation(), GetActorLocation());

	// 기본으로는 Attack1 ~ Attack3 중 하나로 공격
	int RandomIdx = FMath::RandRange(1, 3);
	FName SectionName = *FString::Printf(TEXT("Attack%d"), RandomIdx);

	// 거리가 좀 멀면 달려와서 공격
	if (DistToTarget > GetAttackRange())
	{
		SectionName = TEXT("RunningAttack");
	}

	// HP 반 이하로 떨어지면 확률적으로 콤보 공격
	
	if (mStats->GetStatRatio(TEXT("Hp")) <= 0.5f)
	{
		float Chance = FMath::RandRange(0.f, 100.f);
		if (Chance <= 40.f)
		{
			SectionName = TEXT("ComboAttack");
		}
	}		

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, SectionName.ToString());
	AnimInstance->PlayMontage(TEXT("Attack"), SectionName);
}

void AStelaeKnight::Die()
{
	Super::Die();

	UBossMonsterAnimInstance* AnimInstance = Cast<UBossMonsterAnimInstance>(mAnimInstance);
	if (nullptr == AnimInstance)	return;
	AnimInstance->SetDead(true);
}

void AStelaeKnight::ResetAttackedCharacters()
{
	if (nullptr == mHitInterface)	return;
	mHitInterface->SetDamaged(false);
	mHitInterface = nullptr;
}
