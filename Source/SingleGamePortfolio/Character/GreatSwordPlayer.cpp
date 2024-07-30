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
	
	// 이전 프레임 검 중앙 위치
	FVector Start = mWeapon->GetPrevCollisionPos();
	// 현재 프레임 검 중앙 위치
	FVector End = (mWeapon->GetCollisionStartPos() + mWeapon->GetCollisonEndPos()) * 0.5f;
		
	if (Start == End) return;

	float Radius = (mWeapon->GetCollisionStartPos() - mWeapon->GetCollisionRadiusPos()).Length();
	FVector Origin = (Start + End) * 0.5f;

	FCollisionQueryParams Params(NAME_None, false, this);
	TArray<FHitResult> HitResults;
	bool Collision = GetWorld()->SweepMultiByChannel(HitResults,
		Start, End, FRotationMatrix::MakeFromXZ(GetActorForwardVector(), (End - Start)).ToQuat(),
		ECollisionChannel::ECC_GameTraceChannel3,
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

				//AttackedCharacter->Attacked();

			}
		}
	}


#if ENABLE_DRAW_DEBUG
	FColor DrawColor = Collision ? FColor::Green : FColor::Red;
	//const float CapsuleHalfHeight = (End - Start).Length() * 0.5f + Radius;

	//DrawDebugCapsule(GetWorld(), Origin, CapsuleHalfHeight,
	//	Radius, FRotationMatrix::MakeFromZ((End - Start)).ToQuat(),
	//	DrawColor, false, 1);

	DrawDebugBox(GetWorld(), Origin,
		//FVector(100.f, 50.f, 100.f),
		FVector((mWeapon->GetCollisonEndPos() - mWeapon->GetCollisionStartPos()).Length() * 0.5,
			Radius, (Origin - Start).Length() + Radius),
		FRotationMatrix::MakeFromXZ(GetActorForwardVector(), (End - Start)).ToQuat(),
		DrawColor, false, 1.f);
#endif
	// 다음 프레임을 위해 현재 프레임 검 중앙 위치 저장
	mWeapon->SetPrevCollisionPos
	((mWeapon->GetCollisonEndPos() + mWeapon->GetCollisionStartPos()) * 0.5f);
}


