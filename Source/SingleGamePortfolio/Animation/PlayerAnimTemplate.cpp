// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimTemplate.h"
#include "Kismet/GameplayStatics.h"
#include "Character/PlayerCharacter.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UPlayerAnimTemplate::UPlayerAnimTemplate()
{
}

void UPlayerAnimTemplate::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	mOwningCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
}

void UPlayerAnimTemplate::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (mOwningCharacter)
	{
		UCharacterMovementComponent* Movement = mOwningCharacter->GetCharacterMovement();

		mVelocity = mOwningCharacter->GetVelocity();
		FVector AccelerationLength = Movement->GetCurrentAcceleration();
		AccelerationLength.Z = 0;
		mMoveSpeed = FMath::Min(FVector(mVelocity.X, mVelocity.Y, 0).Length(), AccelerationLength.Length());
		bShouldMove = mMoveSpeed > 5.f;
		
		bIsFalling = Movement->IsFalling();
		bCanJump = mOwningCharacter->GetJumpEnable();
		
		//float Direction = CalculateDirection(mVelocity, mOwningCharacter->GetActorRotation());
		//mWalkForward = mMoveSpeed * FMath::Cos(Direction);
		//mWalkRight = mMoveSpeed * FMath::Sin(Direction);
		mWalkForward = mOwningCharacter->GetMoveVector().X;
		mWalkRight = mOwningCharacter->GetMoveVector().Y;

		bIsRotating = !mOwningCharacter->GetLookInputVector().IsNearlyZero(0.1);
	}
}

void UPlayerAnimTemplate::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OnMontageEnded.AddDynamic(this, &UPlayerAnimTemplate::MontageEnd);
}

void UPlayerAnimTemplate::SetAnimData(const FName& Name)
{
	auto GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI)
	{
		const FPlayerAnimData* AnimData = GI->GetPlayerAnimData(Name);
		if (AnimData)
		{
			mSequenceMap = AnimData->mSequenceMap;
			mBlendSpaceMap = AnimData->mBlendSpaceMap;
			mMontageMap = AnimData->mMontageMap;
			mComboMap = AnimData->mComboMap;
		}
	}
}

void UPlayerAnimTemplate::PlayMontage(const FName& Name, const FName& SectionName)
{
	UAnimMontage** Montage = mMontageMap.Find(Name);
	//if (Montage_IsPlaying(*Montage) && 
	//	!SectionName.Compare(Montage_GetCurrentSection()))	return;
	if (Montage)
	{
		Montage_Play(*Montage);
		Montage_JumpToSection(SectionName);
	}
}

FName UPlayerAnimTemplate::GetNextAttackSection(const FName& CurrnetAttackName, bool IsWeak)
{
	FNextAttack* NextAttack = mComboMap.Find(CurrnetAttackName);
	if (NextAttack)
	{
		if (IsWeak)	return NextAttack->WeakAttack;
		else		return NextAttack->StrongAttack;
	}
	return TEXT("");
}

void UPlayerAnimTemplate::MontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	if (!IsValid(mOwningCharacter))	return;
	if (*mMontageMap.Find(TEXT("Dash")) == Montage)
	{
		mOwningCharacter->SetDodgeEnable(true);
		mOwningCharacter->SetAttackEnable(true);
		mOwningCharacter->SetCurrnetAttack(TEXT("Idle"));
		mOwningCharacter->SetJumpEnable(true);
		mOwningCharacter->SetRunEnable(true);
		return;
	}
	if (*mMontageMap.Find(TEXT("Attack")) == Montage)
	{
		if (bInterrupted)
		{
			//mOwningCharacter->SetJumpEnable(true);
		}
		else
		{
			mOwningCharacter->SetAttackEnable(true);
			mOwningCharacter->SetCurrnetAttack(TEXT("Idle"));
		}
		return;
	}
}

void UPlayerAnimTemplate::AnimNotify_GrabWeapon()
{
	if (!mOwningCharacter)	return;
	mOwningCharacter->SetAttackEnable(true);
	mOwningCharacter->GrabWeapon();
}

void UPlayerAnimTemplate::AnimNotify_Holster()
{
	if (!mOwningCharacter)	return;
	mOwningCharacter->HolsterWeapon();
	if (mOwningCharacter->GetVelocity().SquaredLength() > 100.f)
		Montage_Stop(0.1f, GetCurrentActiveMontage());
}

void UPlayerAnimTemplate::AnimNotify_ComboEnable()
{
	if (!IsValid(mOwningCharacter))	return;
	mOwningCharacter->SetAttackEnable(true);
	mOwningCharacter->SetDodgeEnable(true);
	mOwningCharacter->SetRunEnable(true);
}

void UPlayerAnimTemplate::AnimNotify_ComboDisable()
{
	if (!IsValid(mOwningCharacter))	return;
	mOwningCharacter->SetAttackEnable(false);
	mOwningCharacter->SetJumpEnable(true);
}

void UPlayerAnimTemplate::AnimNotify_ComboEnd()
{
	if (!IsValid(mOwningCharacter))	return;
	mOwningCharacter->SetAttackEnable(true);
	mOwningCharacter->SetDodgeEnable(true);
	mOwningCharacter->SetJumpEnable(true);
	mOwningCharacter->SetRunEnable(true);
}

void UPlayerAnimTemplate::AnimNotify_Jump()
{
	if (!IsValid(mOwningCharacter))	return;
	mOwningCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	mOwningCharacter->SetRunEnable(false);
	mOwningCharacter->SetJumpEnable(false);
}

void UPlayerAnimTemplate::AnimNotify_Walk()
{
	if (!IsValid(mOwningCharacter))	return;
	mOwningCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	mOwningCharacter->SetDodgeEnable(true);
}
