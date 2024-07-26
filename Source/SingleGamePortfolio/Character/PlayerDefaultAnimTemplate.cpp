// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerDefaultAnimTemplate.h"
#include "Kismet/GameplayStatics.h"
#include "../MyGameInstance.h"
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
		}
	}
}

void UPlayerDefaultAnimTemplate::PlayMontage(const FString& Name, const FName& SectionName)
{
	UAnimMontage** Montage = mMontageMap.Find(Name);
	if (Montage)
	{
		Montage_Play(*Montage);
		Montage_JumpToSection(SectionName);

	}
}

void UPlayerDefaultAnimTemplate::MontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	if (*mMontageMap.Find(TEXT("Dash")) == Montage)
	{
		if (IsValid(mOwningCharacter))
		{
			mOwningCharacter->SetIsDodging(false);
		}
	}
}
