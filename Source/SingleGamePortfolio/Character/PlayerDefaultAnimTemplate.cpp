// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerDefaultAnimTemplate.h"
#include "Kismet/GameplayStatics.h"
#include "../BasicCharacter.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UPlayerDefaultAnimTemplate::UPlayerDefaultAnimTemplate()
{
}

void UPlayerDefaultAnimTemplate::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	mOwningCharacter = Cast<ABasicCharacter>(TryGetPawnOwner());
}

void UPlayerDefaultAnimTemplate::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (mOwningCharacter)
	{
		UCharacterMovementComponent* Movement = mOwningCharacter->GetCharacterMovement();

		mVelocity = mOwningCharacter->GetVelocity();
		mMoveSpeed = mVelocity.Length();
		bShouldMove = mMoveSpeed > 5.f;
		
		bIsFalling = Movement->IsFalling();
		
		mWalkForward = mOwningCharacter->GetMoveVector().X;
		mWalkRight = mOwningCharacter->GetMoveVector().Y;
	}
}

void UPlayerDefaultAnimTemplate::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OnMontageEnded.AddDynamic(this, &UPlayerDefaultAnimTemplate::MontageEnd);
}

void UPlayerDefaultAnimTemplate::SetAnimData(const FName& Name)
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

void UPlayerDefaultAnimTemplate::PlayMontage(const FString& Name, const FName& SectionName)
{
	UAnimMontage** Montage = mMontageMap.Find(Name);
	if (Montage_IsPlaying(*Montage) && 
		!SectionName.Compare(Montage_GetCurrentSection()))	return;
	if (Montage)
	{
		Montage_Play(*Montage);
		Montage_JumpToSection(SectionName);
	}
}

FString UPlayerDefaultAnimTemplate::GetNextAttackSection(const FString& CurrnetAttackName, bool IsWeak)
{
	FNextAttack* NextAttack = mComboMap.Find(CurrnetAttackName);
	if (NextAttack)
	{
		if (IsWeak)	return NextAttack->WeakAttack;
		else		return NextAttack->StrongAttack;
	}
	return TEXT("");
}

void UPlayerDefaultAnimTemplate::MontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	if (!IsValid(mOwningCharacter))	return;
	if (*mMontageMap.Find(TEXT("Dash")) == Montage)
	{
		mOwningCharacter->SetIsDodging(false);
		mOwningCharacter->SetAttackEnable(true);
		mOwningCharacter->SetCurrnetAttack(TEXT("Idle"));
		mOwningCharacter->SetJumpEnable(true);
		return;
	}
	if (*mMontageMap.Find(TEXT("Attack")) == Montage && !bInterrupted)
	{
		mOwningCharacter->SetAttackEnable(true);
		mOwningCharacter->SetCurrnetAttack(TEXT("Idle"));
		return;
	}
}

void UPlayerDefaultAnimTemplate::AnimNotify_AttackStart()
{

}

void UPlayerDefaultAnimTemplate::AnimNotify_AttackEnd()
{
	if (!IsValid(mOwningCharacter))	return;

}

void UPlayerDefaultAnimTemplate::AnimNotify_ComboEnable()
{
	if (!IsValid(mOwningCharacter))	return;
	mOwningCharacter->SetAttackEnable(true);
}

void UPlayerDefaultAnimTemplate::AnimNotify_ComboDisable()
{
	if (!IsValid(mOwningCharacter))	return;
	mOwningCharacter->SetAttackEnable(false);
	mOwningCharacter->SetJumpEnable(true);
}

void UPlayerDefaultAnimTemplate::AnimNotify_ComboEnd()
{
	if (!IsValid(mOwningCharacter))	return;
	mOwningCharacter->SetAttackEnable(true);
	mOwningCharacter->SetCurrnetAttack(TEXT("Idle"));
	mOwningCharacter->SetJumpEnable(true);
}

void UPlayerDefaultAnimTemplate::AnimNotify_Jump()
{
	if (!IsValid(mOwningCharacter))	return;
	mOwningCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
}

void UPlayerDefaultAnimTemplate::AnimNotify_Walk()
{
	if (!IsValid(mOwningCharacter))	return;
	mOwningCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}
