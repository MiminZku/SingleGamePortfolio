// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FNextAttack
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName WeakAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName StrongAttack;
};

USTRUCT(BlueprintType)
struct FPlayerAnimData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FName, UAnimSequence*> mSequenceMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FName, UBlendSpace*> mBlendSpaceMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FName, UAnimMontage*> mMontageMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FName, FNextAttack> mComboMap;
};

USTRUCT(BlueprintType)
struct FMonsterAnimData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FName, UAnimSequence*> mSequenceMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FName, UBlendSpace*> mBlendSpaceMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FName, UAnimMontage*> mMontageMap;
};

USTRUCT(BlueprintType)
struct FCharacterStatData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxMp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Atk;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Def;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxExp;
};

UCLASS()
class SINGLEGAMEPORTFOLIO_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UMyGameInstance();
	
public:
	virtual void Init() override;

	FPlayerAnimData* GetPlayerAnimData(const FName& CharacterName) const;
	FMonsterAnimData* GetMonsterAnimData(const FName& MonsterName) const;
	FCharacterStatData* GetCharacterStatData(const FName& Level) const;

private:
	UPROPERTY()
	TObjectPtr<UDataTable> mPlayerAnims;

	UPROPERTY()
	TObjectPtr<UDataTable> mMonsterAnims;

	UPROPERTY()
	TObjectPtr<UDataTable> mCharactetStats;
};
