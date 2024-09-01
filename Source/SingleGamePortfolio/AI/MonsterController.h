// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MonsterController.generated.h"

/**
 * 
 */
UCLASS()
class SINGLEGAMEPORTFOLIO_API AMonsterController : public AAIController
{
	GENERATED_BODY()
	
public:
	AMonsterController();

	void RunAI();
	void StopAI();

protected:
	virtual void OnPossess(APawn* InPawn) override;

protected:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> mBB;

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> mBT;

};
