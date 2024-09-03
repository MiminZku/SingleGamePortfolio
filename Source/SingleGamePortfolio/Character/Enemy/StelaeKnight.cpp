// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/StelaeKnight.h"
#include "Components/CapsuleComponent.h"
#include "AI/BossMonsterController.h"
#include "CharacterStat/CharacterStatComponent.h"
#include "Character/Enemy/MonsterSpawner.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/BossMonsterAnimInstance.h"

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

	//if (IsValid(mSpawner))
	//{
	//	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, TEXT("Bind Delegate"));
	//	mSpawner->OnTargetOverlap.AddUObject(this, &AStelaeKnight::SetTargetPlayer);
	//}
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