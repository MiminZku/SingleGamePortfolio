// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

AMyAIController::AMyAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData>
		BB(TEXT("/Script/AIModule.BlackboardData'/Game/_Programming/AI/BB_Monster.BB_Monster'"));
	if (BB.Object)
	{
		mBB = BB.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree>
		BT(TEXT("/Script/AIModule.BehaviorTree'/Game/_Programming/AI/BT_Monster.BT_Monster'"));
	if (BT.Object)
	{
		mBT = BT.Object;
	}

	
}

void AMyAIController::RunAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (UseBlackboard(mBB, BlackboardPtr))
	{
		Blackboard->SetValueAsVector(TEXT("PatrolPivot"), GetPawn()->GetActorLocation());

		bool RunResult = RunBehaviorTree(mBT);
		ensure(RunResult);
	}
}

void AMyAIController::StopAI()
{
	UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComp)
	{
		BTComp->StopTree();
	}
}

void AMyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAI();
}
