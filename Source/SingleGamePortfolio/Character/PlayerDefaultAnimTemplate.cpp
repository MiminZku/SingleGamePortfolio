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
		
		FVector MoveDir = mOwningCharacter->GetVelocity();
		FVector LookDir = mOwningCharacter->GetActorRotation().Vector();
		MoveDir.Normalize();
		LookDir.Normalize();
		float Degree = FMath::Acos(FVector::DotProduct(MoveDir, LookDir));
		mWalkForward = mVelocity.X * FMath::Cos(Degree);
		mWalkRight = mVelocity.Y * FMath::Sin(Degree);
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.05, FColor::Black,
		//	FString::Printf(TEXT("%f\t\t%f"), mWalkForward, mWalkRight));
	}
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
