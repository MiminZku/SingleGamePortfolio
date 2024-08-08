// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_CanAttack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTDecorator_CanAttack::UBTDecorator_CanAttack()
{
    NodeName = TEXT("CanAttack");
}

bool UBTDecorator_CanAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

    APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
    if (nullptr == ControllingPawn)
    {
        return false;
    }

    APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
    if (nullptr == Target)
    {
        return false;
    }

    float DistanceToTarget = ControllingPawn->GetDistanceTo(Target);
    float AttackRange = 50.f;
    float AttackRadius = 50.f;  // 공격 구체 반지름
    float FinalAttackRange = AttackRange + AttackRadius * 2;

    return (DistanceToTarget <= FinalAttackRange);
}
