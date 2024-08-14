// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/MonsterAnimTemplate.h"
#include "Character/Enemy/MonsterBase.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

UMonsterAnimTemplate::UMonsterAnimTemplate()
{

}

void UMonsterAnimTemplate::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	mOwningCharacter = Cast<AMonsterBase>(TryGetPawnOwner());
}

void UMonsterAnimTemplate::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Cyan,
	//	TEXT("Anim Tick"));
	if (mOwningCharacter)
	{
		UCharacterMovementComponent* Movement = mOwningCharacter->GetCharacterMovement();

		mVelocity = mOwningCharacter->GetVelocity();
		mMoveSpeed = FVector(mVelocity.X, mVelocity.Y, 0).Length();
		bShouldMove = mMoveSpeed > 5.f;
		bIsFalling = Movement->IsFalling();
	}
}

void UMonsterAnimTemplate::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OnMontageEnded.AddDynamic(this, &UMonsterAnimTemplate::MontageEnd);
}

void UMonsterAnimTemplate::SetAnimData(const FName& Name)
{
	auto GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI)
	{
		const FMonsterAnimData* AnimData = GI->GetMonsterAnimData(Name);
		if (AnimData)
		{
			mSequenceMap = AnimData->mSequenceMap;
			mBlendSpaceMap = AnimData->mBlendSpaceMap;
			mMontageMap = AnimData->mMontageMap;
		}
	}
}

void UMonsterAnimTemplate::PlayMontage(const FName& Name, const FName& SectionName)
{
	UAnimMontage** Montage = mMontageMap.Find(Name);
	if (Montage)
	{
		Montage_Play(*Montage);
		Montage_JumpToSection(SectionName);
	}
}

void UMonsterAnimTemplate::MontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	if (!IsValid(mOwningCharacter))	return;
	if (*mMontageMap.Find(TEXT("Attack")) == Montage)
	{
		mOwningCharacter->OnAttackFinished.ExecuteIfBound();

		return;
	}
	if (*mMontageMap.Find(TEXT("Angry")) == Montage)
	{
		mOwningCharacter->OnAngryFinished.ExecuteIfBound();

		return;
	}

}
