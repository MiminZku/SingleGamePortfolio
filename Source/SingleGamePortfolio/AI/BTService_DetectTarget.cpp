// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_DetectTarget.h"
#include "MyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"
#include "Character/Enemy/MonsterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTService_DetectTarget::UBTService_DetectTarget()
{
	NodeName = TEXT("DetectTarget");
	Interval = 0.5f;
}

void UBTService_DetectTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return;
	}

	FVector Center = ControllingPawn->GetActorLocation();
	UWorld* World = ControllingPawn->GetWorld();
	if (nullptr == World)
	{
		return;
	}

	float DetectRadius = 400.f;

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams Param(NAME_None, false, ControllingPawn);
	bool bCollision = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel4,
		FCollisionShape::MakeSphere(DetectRadius),
		Param
	);

	bool bDetect = false;
	if (bCollision)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());
			if (Pawn && Pawn->GetController()->IsPlayerController())
			{
				if (IsInFOV(ControllingPawn, Pawn))
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), Pawn);
					bDetect = true;
					break;
				}
			}
		}
	}

	auto Monster = Cast<AMonsterBase>(ControllingPawn);
	if (nullptr == Monster)	return;

	if (bDetect)
	{
		Monster->SetState(EMonsterState::Trace);
		Monster->GetCharacterMovement()->MaxWalkSpeed = 400.f;
		DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
	}
	else
	{
		Monster->SetState(EMonsterState::Patrol);
		Monster->GetCharacterMovement()->MaxWalkSpeed = 200.f;
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), nullptr);
		DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
	}
}

bool UBTService_DetectTarget::IsInFOV(APawn* ControllingPawn, APawn* DetectedPawn)
{
	float DetectDegree = 120.f;

	FVector ForwardVec = ControllingPawn->GetActorForwardVector();
	FVector VecToTarget = 
		DetectedPawn->GetActorLocation() - ControllingPawn->GetActorLocation();
	ForwardVec.Normalize();
	VecToTarget.Z = 0;	VecToTarget.Normalize();

	float CosTheta = FVector::DotProduct(ForwardVec, VecToTarget);
	float Angle = FMath::RadiansToDegrees(FMath::Acos(CosTheta));
	if (Angle <= DetectDegree / 2)
	{
		return true;
	}
	return false;
}


