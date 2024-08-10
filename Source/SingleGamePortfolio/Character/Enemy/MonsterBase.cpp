// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/MonsterBase.h"
#include "AI/MyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BlackboardComponent.h"


AMonsterBase::AMonsterBase()
{
	AIControllerClass = AMyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCharacterMovement()->MaxWalkSpeed = 200.f;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f);
	GetCharacterMovement()->bUseRVOAvoidance = true; // ¸ó½ºÅÍ³¢¸® ±æ °ãÃÆÀ» ¶§ ºñÄÑ°¡°Ô

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Monster"));
}

float AMonsterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	AMyAIController* Ctrl = Cast<AMyAIController>(GetController());
	if (Ctrl)
	{
		Ctrl->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"),
			EventInstigator->GetPawn());
	}
	if (mAnimInstance)
	{
		UMonsterAnimTemplate* AnimInstance = Cast<UMonsterAnimTemplate>(mAnimInstance);
		if (AnimInstance)
		{
			SetState(EMonsterState::Trace);
			AnimInstance->PlayMontage(TEXT("Hit"), TEXT("Default"));
		}
	}

	return Damage;
}

void AMonsterBase::AttackCollisionCheck(EAttackType AttackType)
{

}

void AMonsterBase::AttackCollisionCheckOnce(FVector Offset, float Radius, EAttackType AttackType)
{

}

void AMonsterBase::Angry()
{
	if (mAnimInstance)
	{
		UMonsterAnimTemplate* AnimInstance = Cast<UMonsterAnimTemplate>(mAnimInstance);
		if (AnimInstance)
		{
			AnimInstance->PlayMontage(TEXT("Angry"), TEXT("Default"));
		}
	}
}

void AMonsterBase::Attack()
{
	if (mAnimInstance)
	{
		UMonsterAnimTemplate* AnimInstance = Cast<UMonsterAnimTemplate>(mAnimInstance);
		if (AnimInstance)
		{
			AnimInstance->PlayMontage(TEXT("Attack"), TEXT("Default"));
		}
	}
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
	switch (mState)
	{
	case EMonsterState::Patrol:
		//GetCharacterMovement()->MaxWalkSpeed = 200.f;
		break;
	case EMonsterState::Trace:
		GetCharacterMovement()->MaxWalkSpeed = 400.f;
		break;
	default:
		break;
	}
}

void AMonsterBase::Die()
{
	Super::Die();

	AMyAIController* Ctrl = Cast<AMyAIController>(GetController());
	if (Ctrl)
	{
		Ctrl->StopAI();
	}
	if (mAnimInstance)
	{
		UMonsterAnimTemplate* AnimInstance = Cast<UMonsterAnimTemplate>(mAnimInstance);
		if (AnimInstance)
		{
			AnimInstance->SetDead(true);
		}
	}
}
