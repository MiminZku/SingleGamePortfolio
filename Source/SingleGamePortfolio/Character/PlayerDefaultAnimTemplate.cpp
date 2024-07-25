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
}

void UPlayerDefaultAnimTemplate::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	ABasicCharacter* Player = Cast<ABasicCharacter>(TryGetPawnOwner());
	if (Player)
	{
		mVelocity = Player->GetVelocity();
		mMoveSpeed = mVelocity.Length();
		
		UCharacterMovementComponent* Movement = Player->GetCharacterMovement();
		bIsFalling = Movement->IsFalling();
		bShouldMove = Movement->GetCurrentAcceleration().Length() > 0.f;
	}
}

void UPlayerDefaultAnimTemplate::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
}

void UPlayerDefaultAnimTemplate::NativePostEvaluateAnimation()
{
	Super::NativePostEvaluateAnimation();
}

void UPlayerDefaultAnimTemplate::NativeUninitializeAnimation()
{
	Super::NativeUninitializeAnimation();
}

void UPlayerDefaultAnimTemplate::NativeBeginPlay()
{
	Super::NativeBeginPlay();
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

void UPlayerDefaultAnimTemplate::PlayMontage(const FString& Name)
{
	UAnimMontage** Montage = mMontageMap.Find(Name);
	if (Montage)
	{
		Montage_Play(*Montage);
	}
}
