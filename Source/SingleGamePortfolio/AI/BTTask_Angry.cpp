// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Angry.h"
#include "AIController.h"
#include "Character/Enemy/MonsterBase.h"

UBTTask_Angry::UBTTask_Angry()
{
	NodeName = TEXT("Angry");
}

EBTNodeResult::Type UBTTask_Angry::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	AMonsterBase* Monster = Cast<AMonsterBase>(ControllingPawn);
	if (nullptr == Monster)
	{
		return EBTNodeResult::Failed;
	}

	Monster->OnAngryFinished.BindLambda(
		[&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);
	Monster->Angry();
	return EBTNodeResult::InProgress;
}
