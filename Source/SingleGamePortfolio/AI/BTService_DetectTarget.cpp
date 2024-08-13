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
	RandomDeviation = 0.f;
}

void UBTService_DetectTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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
	FVector Center = ControllingPawn->GetActorLocation();
	UWorld* World = ControllingPawn->GetWorld();
	if (nullptr == World)
	{
		return;
	}

	float DetectRadius = 2000.f;

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
				AMonsterBase* Monster = Cast<AMonsterBase>(ControllingPawn);
				if (IsInFOV(ControllingPawn, Pawn))
				{
					Monster->DetectedTarget(Pawn);
					if (Monster->bDrawDebug)
					{
						DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
						DrawDebugLine(World, ControllingPawn->GetActorLocation(), Pawn->GetActorLocation(),
							FColor::Green, false, 0.1f);
					}
					break;
				}
				else
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), nullptr);
					if (Monster->bDrawDebug)
					{
						DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
						DrawDebugLine(World, ControllingPawn->GetActorLocation(), Pawn->GetActorLocation(),
							FColor::Red, false, 0.1f);
					}
				}
			}
		}
	}
}

bool UBTService_DetectTarget::IsInFOV(APawn* ControllingPawn, APawn* DetectedPawn)
{
	// 시야각에 들었는지
	float DetectDegree = 180.f;

	FVector ForwardVec = ControllingPawn->GetActorForwardVector();
	FVector VecToTarget = 
		DetectedPawn->GetActorLocation() - ControllingPawn->GetActorLocation();
	ForwardVec.Normalize();
	VecToTarget.Z = 0;	VecToTarget.Normalize();

	float CosTheta = FVector::DotProduct(ForwardVec, VecToTarget);
	float Angle = FMath::RadiansToDegrees(FMath::Acos(CosTheta));
	if (Angle > DetectDegree / 2)
	{
		return false;
	}

	// 중간에 물체에 가려졌는지
	UWorld* World = ControllingPawn->GetWorld();
	FHitResult HitResult;
	FCollisionQueryParams Param(NAME_None, false, ControllingPawn);
	
	bool bCollision = World->LineTraceSingleByChannel(HitResult, 
		ControllingPawn->GetActorLocation(), DetectedPawn->GetActorLocation(),
		ECollisionChannel::ECC_Visibility, Param);
	if (bCollision)
	{
		APawn* Pawn = Cast<APawn>(HitResult.GetActor());
		if (DetectedPawn != Pawn)
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	return true;
}


