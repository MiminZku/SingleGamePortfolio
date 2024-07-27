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
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));

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


