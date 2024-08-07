// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/MonsterBase.h"
#include "AI/MyAIController.h"

AMonsterBase::AMonsterBase()
{
	AIControllerClass = AMyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AMonsterBase::AttackCollisionCheck(EAttackType AttackType)
{

}

void AMonsterBase::AttackCollisionCheckOnce(FVector Offset, float Radius, EAttackType AttackType)
{

}
