// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/CharacterStatComponent.h"

// Sets default values for this component's properties
UCharacterStatComponent::UCharacterStatComponent()
{
	mMaxHp = 100.f;
	SetHp(mMaxHp);
}


// Called when the game starts
void UCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	SetHp(mMaxHp);
}

float UCharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = mCurHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);

	SetHp(PrevHp - ActualDamage);
	if (mCurHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}
	return ActualDamage;
}

void UCharacterStatComponent::SetHp(float NewHp)
{
	mCurHp = FMath::Clamp<float>(NewHp, 0.f, mMaxHp);
	OnHpChanged.Broadcast();
}

