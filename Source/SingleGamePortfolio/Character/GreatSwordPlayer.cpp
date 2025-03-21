// Fill out your copyright notice in the Description page of Project Settings.


#include "GreatSwordPlayer.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/PlayerAnimTemplate.h"
#include "Item/PlayerWeapon.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/DamageEvents.h"
#include "Item/ItemBox.h"
#include "Interface/HitInterface.h"
#include "CharacterStat/CharacterStatComponent.h"

AGreatSwordPlayer::AGreatSwordPlayer()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		MeshAsset(TEXT("/Script/Engine.SkeletalMesh'/Game/_ART/Character/Hercules/Hercules.Hercules'"));
	if (MeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMeshAsset(MeshAsset.Object);
	}
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -95.f));
	GetMesh()->SetRelativeScale3D(FVector(1.25f, 1.25f, 1.25f));

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
	Super::PostInitializeComponents();

	mAnimInstance = GetMesh()->GetAnimInstance();
	UPlayerAnimTemplate* AnimInstance = Cast<UPlayerAnimTemplate>(mAnimInstance);
	if (AnimInstance)
	{
		AnimInstance->SetAnimData(TEXT("Hercules"));
	}
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
	mWeapon->GetMesh()->AttachToComponent(GetMesh(),
		FAttachmentTransformRules::SnapToTargetIncludingScale,
		TEXT("hand_r_weapon"));
}

void AGreatSwordPlayer::HolsterWeapon()
{
	Super::HolsterWeapon();
	FAttachmentTransformRules Rules(
		EAttachmentRule::KeepWorld,
		EAttachmentRule::KeepWorld,
		EAttachmentRule::KeepRelative,
		true);
	mWeapon->GetMesh()->AttachToComponent(GetMesh(),
		FAttachmentTransformRules::KeepWorldTransform,
		TEXT("unequiped_weapon"));

	FLatentActionInfo Info;
	Info.CallbackTarget = this;
	UKismetSystemLibrary::MoveComponentTo(mWeapon->GetMesh(),
		FVector::ZeroVector, FRotator::ZeroRotator,
		true, true, 0.2f, false,
		EMoveComponentAction::Type::Move, Info);
}

void AGreatSwordPlayer::AttackCollisionCheck(EAttackType AttackType)
{
	Super::AttackCollisionCheck(AttackType);

	APlayerWeapon* Weapon = GetWeapon();
	if (!Weapon)	return;
	
	// 이전 프레임 검 중앙 위치
	FVector Start = mWeapon->GetPrevCollisionPos();
	// 현재 프레임 검 중앙 위치
	FVector End = (mWeapon->GetCollisionStartPos() + mWeapon->GetCollisionEndPos()) * 0.5f;
		
	if (Start == End) return;

	float Radius = (mWeapon->GetCollisionStartPos() - mWeapon->GetCollisionRadiusPos()).Length();

	FCollisionQueryParams Params(NAME_None, false, this);
	TArray<FHitResult> HitResults;
	bool Collision = GetWorld()->SweepMultiByChannel(OUT HitResults,
		Start, End, FRotationMatrix::MakeFromXZ(GetActorForwardVector(), (End - Start)).ToQuat(),
		ECollisionChannel::ECC_GameTraceChannel3,
		FCollisionShape::MakeBox(
		FVector((mWeapon->GetCollisionEndPos() - mWeapon->GetCollisionStartPos()).Length() * 0.5f,
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
				mHitInterfaces.Add(HitInterface);
				
				HitStop(0.2f, 0.05f);

				FDamageEvent DmgEvent;
				HitResult.GetActor()->TakeDamage(mStats->GetAtk(), DmgEvent, GetController(), mWeapon);

				HitInterface->Execute_GetHit(HitResult.GetActor(), GetActorLocation());
				HitInterface->Execute_HitEvent(HitResult.GetActor(), GetActorLocation(), AttackType);

				AItemBox* ItemBox = Cast<AItemBox>(HitResult.GetActor());
				if (ItemBox)
				{
					Weapon->CreateFields(HitResult.ImpactPoint);
					FTimerHandle TimerHandle;
					GetWorld()->GetTimerManager().SetTimer(TimerHandle,
						[&, Weapon, HitResult]()
						{
							Weapon->CreateFields(HitResult.ImpactPoint);
						},
						0.01f, false);
				}
			}
		}
	}

	// 다음 프레임을 위해 현재 프레임 검 중앙 위치 저장
	mWeapon->SetPrevCollisionPos(End);


#if ENABLE_DRAW_DEBUG
	if (bDrawDebug)
	{
		FVector Origin = (Start + End) * 0.5f;
		FColor DrawColor = Collision ? FColor::Green : FColor::Red;
		//const float CapsuleHalfHeight = (End - Start).Length() * 0.5f + Radius;

		//DrawDebugCapsule(GetWorld(), Origin, CapsuleHalfHeight,
		//	Radius, FRotationMatrix::MakeFromZ((End - Start)).ToQuat(),
		//	DrawColor, false, 1);

		DrawDebugBox(GetWorld(), Origin,
			FVector((mWeapon->GetCollisionEndPos() - mWeapon->GetCollisionStartPos()).Length() * 0.5,
				Radius, (Origin - Start).Length() + Radius),
			FRotationMatrix::MakeFromXZ(GetActorForwardVector(), (End - Start)).ToQuat(),
			DrawColor, false, 1.f);
	}
#endif
}


