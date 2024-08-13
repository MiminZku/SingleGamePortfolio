// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/MonsterBase.h"
#include "AI/MyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Enemy/MonsterSpawner.h"
#include "CharacterStat/CharacterStatComponent.h"

AMonsterBase::AMonsterBase()
{
	AIControllerClass = AMyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCharacterMovement()->MaxWalkSpeed = 200.f;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f);
	GetCharacterMovement()->bUseRVOAvoidance = true; // ¸ó½ºÅÍ³¢¸® ±æ °ãÃÆÀ» ¶§ ºñÄÑ°¡°Ô
	GetCharacterMovement()->AvoidanceConsiderationRadius = 100.f;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Monster"));

	mTarget = nullptr;
}

float AMonsterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	DetectedTarget(EventInstigator->GetPawn());

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

void AMonsterBase::AttackCollisionCheck()
{

}

void AMonsterBase::AttackCollisionCheckOnce(FVector Offset, float Radius)
{

}

void AMonsterBase::Activate()
{
	SetActorHiddenInGame(false);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	AMyAIController* Ctrl = Cast<AMyAIController>(GetController());
	if (Ctrl)
	{
		Ctrl->RunAI();
	}
	mStats->Rebirth();
	SetHpBarVisible(false);
}

void AMonsterBase::Deactivate()
{
	SetActorHiddenInGame(true);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
			AnimInstance->SetDead(false);
		}
	}
	RegisterTarget(nullptr);
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
	GetCharacterMovement()->bUseRVOAvoidance = false;
}

void AMonsterBase::BindSpawner(AMonsterSpawner* Spawner)
{
	mSpawner = Spawner;
	if (IsValid(mSpawner))
		mSpawner->OnDetectTarget.AddUObject(this, &AMonsterBase::RegisterTarget);
}

void AMonsterBase::DetectedTarget(APawn* Target)
{
	RegisterTarget(Target);
	mSpawner->DetectedTarget(Target);
}

void AMonsterBase::RegisterTarget(APawn* Target)
{
	SetTarget(Target);
	SetState(EMonsterState::Trace);
	AMyAIController* Ctrl = Cast<AMyAIController>(GetController());
	if (Ctrl)
	{
		Ctrl->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), Target);
	}
}