// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_RotateToTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_RotateToTarget::UBTTask_RotateToTarget()
{

}

EBTNodeResult::Type UBTTask_RotateToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	APawn* TargetPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
	if (nullptr == TargetPawn)
	{
		return EBTNodeResult::Failed;
	}

	float RotateSpeed = 2.f;
	FVector LookVector = TargetPawn->GetActorLocation() - ControllingPawn->GetActorLocation();
	LookVector.Z = 0.f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	ControllingPawn->SetActorRotation(FMath::RInterpTo(ControllingPawn->GetActorRotation(), TargetRot,
		GetWorld()->GetDeltaSeconds(), RotateSpeed));

	return EBTNodeResult::Succeeded;
}
