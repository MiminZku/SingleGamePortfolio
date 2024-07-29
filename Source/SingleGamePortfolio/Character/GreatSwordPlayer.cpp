// Fill out your copyright notice in the Description page of Project Settings.


#include "GreatSwordPlayer.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerDefaultAnimTemplate.h"
#include "../Item/PlayerWeapon.h"

AGreatSwordPlayer::AGreatSwordPlayer()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		MeshAsset(TEXT("/Script/Engine.SkeletalMesh'/Game/_ART/Character/Hercules/Hercules.Hercules'"));
	if (MeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMeshAsset(MeshAsset.Object);
	}
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -95.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetRelativeScale3D(FVector(1.25f, 1.25f, 1.25f));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->bReceivesDecals = false;

	GetCapsuleComponent()->SetCapsuleHalfHeight(95.f);

	static ConstructorHelpers::FClassFinder<UAnimInstance>
		AnimClass(TEXT("/Script/Engine.AnimBlueprint'/Game/_Programming/Character/Hercules/ABP_Hercules.ABP_Hercules_C'"));
	if (AnimClass.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimClass.Class);
	}
}

void AGreatSwordPlayer::PostInitializeComponents()
{
	mAnimInstance = Cast<UPlayerDefaultAnimTemplate>(GetMesh()->GetAnimInstance());
	if (mAnimInstance)
	{
		mAnimInstance->SetAnimData(TEXT("Hercules"));
	}
	Super::PostInitializeComponents();
}

void AGreatSwordPlayer::BeginPlay()
{
	Super::BeginPlay();

	FVector Start = GetActorLocation(); // 시작 위치
	FVector End = Start + FVector(100.f, 0.f, 50.f); // 끝 위치 (Z 값이 다름)
	FQuat Rotation = FQuat::Identity; // 회전 값 (회전이 필요 없다면 Identity 사용)

	// Box의 크기
	FVector BoxExtent = FVector(50.f, 50.f, 50.f);

	// Sweep 결과
	FHitResult HitResult;

	// Sweep을 수행
	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		Start,
		End,
		Rotation,
		ECC_Visibility, // 사용할 채널
		FCollisionShape::MakeBox(BoxExtent) // Box Collision Shape 생성
	);

	// 평행사변형 꼭짓점 계산
	FVector Offset = FVector(0, 0, BoxExtent.Z);
	FVector Point1 = Start - Offset;  // 시작점 아래쪽
	FVector Point2 = Start + Offset;  // 시작점 위쪽
	FVector Point3 = End - Offset;    // 끝점 아래쪽
	FVector Point4 = End + Offset;    // 끝점 위쪽

	// 평행사변형을 그리기
	if (bHit)
	{
		// 충돌이 발생한 경우, 초록색 평행사변형
		DrawDebugLine(GetWorld(), Point1, Point2, FColor::Green, true);
		DrawDebugLine(GetWorld(), Point3, Point4, FColor::Green, true);
		DrawDebugLine(GetWorld(), Point1, Point3, FColor::Green, true);
		DrawDebugLine(GetWorld(), Point2, Point4, FColor::Green, true);
	}
	else
	{
		// 충돌이 발생하지 않은 경우, 빨간색 평행사변형
		DrawDebugLine(GetWorld(), Point1, Point2, FColor::Red, true);
		DrawDebugLine(GetWorld(), Point3, Point4, FColor::Red, true);
		DrawDebugLine(GetWorld(), Point1, Point3, FColor::Red, true);
		DrawDebugLine(GetWorld(), Point2, Point4, FColor::Red, true);
	}
}

void AGreatSwordPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//AttackCollisionCheck();
}

void AGreatSwordPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float AGreatSwordPlayer::TakeDamage(float DamageAmount,
	FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	DamageAmount = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator,
		DamageCauser);

	return DamageAmount;
}

void AGreatSwordPlayer::Arm()
{
	if (!bHasWeapon)
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red,
			TEXT("Pick up your weapon!"));
		return;
	}
	Super::Arm();
	GetCharacterMovement()->MaxWalkSpeed = 200.f;
}

void AGreatSwordPlayer::Unarm()
{
	Super::Unarm();
}

void AGreatSwordPlayer::Attack(bool IsWeak)
{
	Super::Attack(IsWeak);
}

void AGreatSwordPlayer::GrabWeapon()
{
	Super::GrabWeapon();
	mWeapon->AttachToComponent(GetMesh(),
		FAttachmentTransformRules::SnapToTargetIncludingScale,
		TEXT("hand_r_weapon"));
}

void AGreatSwordPlayer::HolsterWeapon()
{
	Super::HolsterWeapon();
	mWeapon->AttachToComponent(GetMesh(),
		FAttachmentTransformRules::SnapToTargetIncludingScale,
		TEXT("unequiped_weapon"));
}

void AGreatSwordPlayer::AttackCollisionCheck()
{
	Super::AttackCollisionCheck();

	APlayerWeapon* Weapon = GetWeapon();
	if (!Weapon)	return;

	FVector Start = mWeapon->GetPrevCollisionPos();
		//mWeapon->GetCollisionStartPos();
	FVector End = (mWeapon->GetCollisionStartPos() + mWeapon->GetCollisonEndPos()) * 0.5f;
		//mWeapon->GetCollisonEndPos();
	float Radius = (Start - mWeapon->GetCollisionRadiusPos()).Length();
	//FVector SwordVec = End - Start;
	//SwordVec.Normalize();
	//Start += SwordVec * Radius;
	//End -= SwordVec * Radius;

	FCollisionQueryParams Params(NAME_None, false, this);
	TArray<FHitResult> HitResults;
	bool Collision = GetWorld()->SweepMultiByChannel(HitResults,
		Start, End, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel3,
		//FCollisionShape::MakeSphere(Radius),
		FCollisionShape::MakeBox(
		FVector((mWeapon->GetCollisonEndPos() - mWeapon->GetCollisionStartPos()).Length() * 0.5f,
			Radius, Radius)), Params);

	if (Collision)
	{
		for (const FHitResult& HitResult : HitResults)
		{
			INormalAttackInterface* AttackedCharacter = Cast<INormalAttackInterface>(HitResult.GetActor());
			if (AttackedCharacter)
			{
				if (AttackedCharacter->IsDamaged())	continue;
				AttackedCharacter->SetDamaged(true);
				mAttackedCharacters.Add(AttackedCharacter);
				if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green,
					FString::Printf(TEXT("%s"), *HitResult.GetActor()->GetName()));
			}
		}
	}

#if ENABLE_DRAW_DEBUG
	FColor DrawColor = Collision ? FColor::Green : FColor::Red;
	FVector Origin = (Start + End) * 0.5f;
	//const float CapsuleHalfHeight = (End - Start).Length() * 0.5f + Radius;
	

	//DrawDebugCapsule(GetWorld(), Origin, CapsuleHalfHeight,
	//	Radius, FRotationMatrix::MakeFromZ((End - Start)).ToQuat(),
	//	DrawColor, false, 1);

	if(End - Start != FVector::ZeroVector)
	DrawDebugBox(GetWorld(), Origin, 
		FVector((mWeapon->GetCollisonEndPos() - mWeapon->GetCollisionStartPos()).Length() * 0.5,
			(Origin - Start).Length(), Radius),
		FRotationMatrix::MakeFromZ((End - Start)).ToQuat(),
		//FQuat::Identity,
		DrawColor, false, 1.f);
#endif
	mWeapon->SetPrevCollisionPos
	((mWeapon->GetCollisonEndPos() + mWeapon->GetCollisionStartPos()) * 0.5f);
}


