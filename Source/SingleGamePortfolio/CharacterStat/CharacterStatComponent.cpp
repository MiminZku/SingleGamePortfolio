// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/CharacterStatComponent.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UCharacterStatComponent::UCharacterStatComponent()
{
	bWantsInitializeComponent = true;
	//mMaxHp = 100.f;
	//SetHp(mMaxHp);
}

void UCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetStats(mCurLevel);
}

// Called when the game starts
void UCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UCharacterStatComponent::SetStats(int32 Level)
{
	FName RowName = *FString::Printf(TEXT("level%d"), Level);
	auto GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI)
	{
		const FCharacterStatData* StatData = GI->GetCharacterStatData(RowName);
		if (StatData)
		{
			mCurLevel = Level;
			mMaxHp = StatData->MaxHp;
			SetHp(mMaxHp);
			mMaxMp = StatData->MaxMp;
			SetMp(mMaxMp);
			mAtk = StatData->Atk;
			mDef = StatData->Def;
			mMaxExp = StatData->MaxExp;
			SetExp(0.f);
			mDropExp = StatData->DropExp;
		}
	}
}

float UCharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = mCurHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage - mDef, 1, InDamage);

	SetHp(PrevHp - ActualDamage);
	return ActualDamage;
}

void UCharacterStatComponent::RecoverHp(float InValue)
{
	SetHp(mCurHp + InValue);
}

void UCharacterStatComponent::SetHp(float NewHp)
{
	mCurHp = FMath::Clamp<float>(NewHp, 0.f, mMaxHp);
	OnHpChanged.Broadcast();
	if (mCurHp <= 0)
	{
		OnHpZero.Broadcast();
	}
}

void UCharacterStatComponent::UseMp(float InValue)
{
	SetMp(mCurMp - InValue);
}

void UCharacterStatComponent::RecoverMp(float InValue)
{
	SetMp(mCurMp + InValue);
}

void UCharacterStatComponent::SetMp(float NewMp)
{
	mCurMp = FMath::Clamp<float>(NewMp, 0.f, mMaxMp);
	OnMpChanged.Broadcast();
}

void UCharacterStatComponent::ExpUp(float InValue)
{
	SetExp(mCurExp + InValue);
}

void UCharacterStatComponent::SetExp(float NewExp)
{
	mCurExp = NewExp;
	OnExpChanged.Broadcast();
	if (mCurExp >= mMaxExp)
	{
		SetStats(mCurLevel + 1);
		OnLevelUp.Broadcast();
	}
}
