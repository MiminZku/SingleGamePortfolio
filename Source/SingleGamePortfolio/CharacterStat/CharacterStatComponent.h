// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterStatComponent.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE(FOnHpChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnMpChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnExpChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnLevelUpDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SINGLEGAMEPORTFOLIO_API UCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterStatComponent();

protected:
	virtual void InitializeComponent() override;

	// Called when the game starts
	virtual void BeginPlay() override;

public:
	FORCEINLINE uint16 GetLevel() { return mCurLevel; }
	//FORCEINLINE float GetMaxHp() { return mMaxHp; }
	//FORCEINLINE float GetCurrentHp() { return mCurHp; }
	//FORCEINLINE float GetMaxMp() { return mMaxMp; }
	//FORCEINLINE float GetCurrentMp() { return mCurMp; }
	//FORCEINLINE float GetMaxExp() { return mMaxExp; }
	//FORCEINLINE float GetCurrentExp() { return mCurExp; }
	FORCEINLINE float GetAtk() { return mAtk; }
	FORCEINLINE float GetDef() { return mDef; }
	FORCEINLINE float GetDropExp() { return mDropExp; }

	FORCEINLINE float GetStatRatio(const FName& StatName)
	{
		if (TEXT("Hp") == StatName) return mCurHp / mMaxHp;
		if (TEXT("Mp") == StatName) return  mCurMp / mMaxMp;
		if (TEXT("Exp") == StatName) return mCurExp / mMaxExp;
		return 0.f;
	}
	//FORCEINLINE float GetHpRatio() { return mCurHp / mMaxHp; }
	//FORCEINLINE float GetMpRatio() { return mCurMp / mMaxMp; }
	//FORCEINLINE float GetExpRatio() { return mCurExp / mMaxExp; }
	FORCEINLINE float GetCurrentStat(const FName& StatName)
	{
		if (TEXT("Hp") == StatName) return mCurHp;
		if (TEXT("Mp") == StatName) return  mCurMp;
		if (TEXT("Exp") == StatName) return mCurExp;
		return 0.f;
	}
	FORCEINLINE float GetMaxStat(const FName& StatName)
	{
		if (TEXT("Hp") == StatName) return mMaxHp;
		if (TEXT("Mp") == StatName) return  mMaxMp;
		if (TEXT("Exp") == StatName) return mMaxExp;
		return 0.f;
	}

	void SetStats(int32 Level);
	
	FORCEINLINE float ApplyDamage(float InDamage);
	void RecoverHp(float InValue);

	FORCEINLINE void UseMp(float InValue);
	void RecoverMp(float InValue);

	FORCEINLINE void ExpUp(float InValue);

protected:
	void SetHp(float NewHp);
	void SetMp(float NewMp);
	void SetExp(float NewExp);

public:
	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;
	FOnMpChangedDelegate OnMpChanged;
	FOnExpChangedDelegate OnExpChanged;
	FOnLevelUpDelegate OnLevelUp;

protected:
	UPROPERTY(EditAnywhere, Category = Stat)
	uint16 mCurLevel = 1;

	UPROPERTY(VisibleInstanceOnly, Category = Stat)
	float mMaxHp;

	UPROPERTY(Transient /* 디스크에 저장할 필요 X */, VisibleInstanceOnly, Category = Stat)
	float mCurHp;

	UPROPERTY(VisibleInstanceOnly, Category = Stat)
	float mMaxMp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float mCurMp;

	UPROPERTY(VisibleInstanceOnly, Category = Stat)
	float mAtk;

	UPROPERTY(VisibleInstanceOnly, Category = Stat)
	float mDef;

	UPROPERTY(VisibleInstanceOnly, Category = Stat)
	float mMaxExp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float mCurExp;

	UPROPERTY(VisibleInstanceOnly, Category = Stat)
	float mDropExp;
};
