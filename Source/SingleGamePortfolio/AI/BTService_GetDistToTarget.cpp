// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_GetDistToTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTService_GetDistToTarget::UBTService_GetDistToTarget()
{
	Interval = 0.1f;
	RandomDeviation = 0.f;
}

void UBTService_GetDistToTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (nullptr != OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Target")))
	{
		return;
	}

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return;
	}


}
