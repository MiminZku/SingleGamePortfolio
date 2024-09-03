// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_IsClose.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Enemy/MonsterBase.h"

UBTDecorator_IsClose::UBTDecorator_IsClose()
{
}

bool UBTDecorator_IsClose::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

    APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
    if (nullptr == ControllingPawn)
    {
        return false;
    }
    float DistToTarget = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TEXT("DistToTarget"));
    
    return true;
}
