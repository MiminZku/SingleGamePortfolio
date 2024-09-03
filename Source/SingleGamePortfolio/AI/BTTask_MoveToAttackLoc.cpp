// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_MoveToAttackLoc.h"
#include "AIController.h"
#include "Character/Enemy/MonsterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "NavigationSystem.h"

UBTTask_MoveToAttackLoc::UBTTask_MoveToAttackLoc()
{
	NodeName = TEXT("MoveToAttackLoc");
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

	if (Monster->IsBoss())
	{
		APawn* Target = Monster->GetTarget();
		if (IsValid(Target))
		{
			
			FRotator TargetRot = (ControllingPawn->GetActorLocation() - Target->GetActorLocation()).Rotation();
			float Degree = FMath::RandRange(-90.f, 90.f);
			float Forward = Monster->GetAttackRange() * FMath::Cos(FMath::DegreesToRadians(Degree));
			float Right = Monster->GetAttackRange() * FMath::Sin(FMath::DegreesToRadians(Degree));

			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan,
				FString::Printf(TEXT("%f"), FMath::Sqrt(Forward * Forward + Right * Right)));
			FVector Location = 
				Target->GetActorLocation() 
				+ UKismetMathLibrary::GetForwardVector(TargetRot) * Forward 
				+ UKismetMathLibrary::GetRightVector(TargetRot) * Right;
			
			//OwnerComp.GetBlackboardComponent()->SetValueAsVector(TEXT("AttackLoc"), Location);

			UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
			if (nullptr == NavSystem)
			{
				return EBTNodeResult::Failed;
			}
			FNavLocation NextPatrolPos;			
			if (NavSystem->GetRandomPointInNavigableRadius(Location, 100.f, OUT NextPatrolPos))
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(TEXT("AttackLoc"), NextPatrolPos.Location);
			}
#if ENABLE_DRAW_DEBUG
			DrawDebugSphere(GetWorld(), NextPatrolPos.Location, 50.f, 8, FColor::Green, false, 3.f);
#endif
		}
	}
	else
	{
		AcceptableRadius = Monster->GetAttackRange() * 0.9f;
	}
	
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
