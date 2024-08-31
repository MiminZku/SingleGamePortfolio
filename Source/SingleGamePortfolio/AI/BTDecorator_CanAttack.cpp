// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_CanAttack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Enemy/MonsterBase.h"
#include "Components/CapsuleComponent.h"

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

    AMonsterBase* Monster = Cast<AMonsterBase>(ControllingPawn);
    if (nullptr == Monster)
    {
        return false;
    }

    float DistanceToTarget = ControllingPawn->GetDistanceTo(Target);
    float AttackRange = Monster->GetAttackRange();
    float FinalAttackRange = 
        Monster->GetCapsuleComponent()->GetScaledCapsuleRadius() + AttackRange;

    return (DistanceToTarget <= FinalAttackRange);
}
