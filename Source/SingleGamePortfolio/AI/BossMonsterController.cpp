// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BossMonsterController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"

ABossMonsterController::ABossMonsterController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData>
		BB(TEXT("/Script/AIModule.BlackboardData'/Game/_Programming/AI/BB_Boss.BB_Boss'"));
	if (BB.Object)
	{
		mBB = BB.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree>
		BT(TEXT("/Script/AIModule.BehaviorTree'/Game/_Programming/AI/BT_Boss.BT_Boss'"));
	if (BT.Object)
	{
		mBT = BT.Object;
	}
}
