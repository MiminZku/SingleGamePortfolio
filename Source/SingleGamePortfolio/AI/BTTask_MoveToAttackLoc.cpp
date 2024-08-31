// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_MoveToAttackLoc.h"
#include "AIController.h"
#include "Character/Enemy/MonsterBase.h"

UBTTask_MoveToAttackLoc::UBTTask_MoveToAttackLoc()
{

}

EBTNodeResult::Type UBTTask_MoveToAttackLoc::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
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

	AcceptableRadius = Monster->GetAttackRange() * 0.9f;
	
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
