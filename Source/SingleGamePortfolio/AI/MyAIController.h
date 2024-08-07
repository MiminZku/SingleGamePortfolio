// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MyAIController.generated.h"

/**
 * 
 */
UCLASS()
class SINGLEGAMEPORTFOLIO_API AMyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AMyAIController();

	void RunAI();
	void StopAI();

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> mBB;

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> mBT;

};
