// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/MonsterBase.h"
#include "AI/MyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"


AMonsterBase::AMonsterBase()
{
	AIControllerClass = AMyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	GetCharacterMovement()->MaxWalkSpeed = 200.f;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Monster"));
}

void AMonsterBase::AttackCollisionCheck(EAttackType AttackType)
{

}

void AMonsterBase::AttackCollisionCheckOnce(FVector Offset, float Radius, EAttackType AttackType)
{

}

void AMonsterBase::Attack()
{
}

void AMonsterBase::SetState(EMonsterState InState)
{
	mState = InState;
	if (mAnimInstance)
	{
		UMonsterAnimTemplate* AnimInstance = Cast<UMonsterAnimTemplate>(mAnimInstance);
		if (AnimInstance)
		{
			AnimInstance->SetState(mState);
		}
	}

}
