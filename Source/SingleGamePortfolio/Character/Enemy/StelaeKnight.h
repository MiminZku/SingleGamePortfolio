// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Enemy/MonsterBase.h"
#include "StelaeKnight.generated.h"

/**
 * 
 */
UCLASS()
class SINGLEGAMEPORTFOLIO_API AStelaeKnight : public AMonsterBase
{
	GENERATED_BODY()
	
public:
	AStelaeKnight();

public:


protected:
	virtual void PostInitializeComponents() override;


protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> mSword;

private:

};
