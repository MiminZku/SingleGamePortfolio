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

USTRUCT()
struct FPlayerAnimData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FName, UAnimSequence*> mSequenceMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FName, UBlendSpace*> mBlendSpaceMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FName, UAnimMontage*> mMontageMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FName, FNextAttack> mComboMap;
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

private:
	UPROPERTY()
	UDataTable* mPlayerAnims = nullptr;
};
