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
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	FORCEINLINE float GetMaxHp() { return mMaxHp; }
	FORCEINLINE float GetCurrentHp() { return mCurHp; }
	FORCEINLINE float GetMaxMp() { return mMaxMp; }
	FORCEINLINE float GetCurrentMp() { return mCurMp; }
	FORCEINLINE float GetAtk() { return mAtk; }
	FORCEINLINE float GetDef() { return mDef; }
	FORCEINLINE float GetMaxExp() { return mMaxExp; }
	FORCEINLINE float GetCurrentExp() { return mCurExp; }
	
	FORCEINLINE void SetHpMax() { SetHp(mMaxHp); }

	FORCEINLINE float GetHpRatio() { return mCurHp / mMaxHp; }
	FORCEINLINE float GetMpRatio() { return mCurMp / mMaxMp; }
	FORCEINLINE float GetExpRatio() { return mCurExp / mMaxExp; }

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
	UPROPERTY(VisibleInstanceOnly /* 캐릭터들 마다 다른 값 */, Category = Stat)
	uint16 mCurLevel;

	UPROPERTY(VisibleInstanceOnly /* 캐릭터들 마다 다른 값 */, Category = Stat)
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
};
