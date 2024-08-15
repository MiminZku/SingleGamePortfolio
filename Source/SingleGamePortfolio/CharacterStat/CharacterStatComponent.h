// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterStatComponent.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE(FOnHpChangedDelegate);

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
	
	FORCEINLINE float GetHpRatio() { return mCurHp / mMaxHp; }
	

	
	float ApplyDamage(float InDamage);
	
	void Rebirth();

protected:
	void SetHp(float NewHp);

public:
	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;

protected:
	UPROPERTY(VisibleInstanceOnly /* 캐릭터들 마다 다른 값 */, Category = Stat)
	float mMaxHp;

	UPROPERTY(Transient /* 디스크에 저장할 필요 X */, VisibleInstanceOnly, Category = Stat)
	float mCurHp;
};
