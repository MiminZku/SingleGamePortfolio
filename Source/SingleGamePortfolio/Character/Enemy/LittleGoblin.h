// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Enemy/MonsterBase.h"
#include "LittleGoblin.generated.h"

/**
 * 
 */
UCLASS()
class SINGLEGAMEPORTFOLIO_API ALittleGoblin : public AMonsterBase
{
	GENERATED_BODY()

public:
	ALittleGoblin();

protected:
	virtual void PostInitializeComponents() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	

	virtual void Angry() override;
	virtual void Attack() override;

};
