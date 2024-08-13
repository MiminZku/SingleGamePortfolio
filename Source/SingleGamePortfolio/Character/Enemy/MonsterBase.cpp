// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/MonsterBase.h"
#include "AI/MyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Enemy/MonsterSpawner.h"
#include "CharacterStat/CharacterStatComponent.h"
#include "Engine/DamageEvents.h"

AMonsterBase::AMonsterBase()
{
	AIControllerClass = AMyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCharacterMovement()->MaxWalkSpeed = 200.f;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f);
	GetCharacterMovement()->bUseRVOAvoidance = true; // ¸ó½ºÅÍ³¢¸® ±æ °ãÃÆÀ» ¶§ ºñÄÑ°¡°Ô
	GetCharacterMovement()->AvoidanceConsiderationRadius = 300.f;

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
	FVector Origin = GetActorLocation() + Offset.X * GetActorForwardVector();
	FCollisionQueryParams Params(NAME_None, false, this);
	TArray<FHitResult> HitResults;
	bool Collision = GetWorld()->SweepMultiByChannel(OUT HitResults,
		Origin, Origin, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel4,
		FCollisionShape::MakeSphere(Radius), Params);

	if (Collision)
	{
		for (const FHitResult& HitResult : HitResults)
		{
			IAttackInterface* AttackedCharacter = Cast<IAttackInterface>(HitResult.GetActor());
			if (AttackedCharacter)
			{
				FDamageEvent DmgEvent;
				HitResult.GetActor()->TakeDamage(5.f, DmgEvent, GetController(), this);
			}
		}
	}

#if ENABLE_DRAW_DEBUG
	if (bDrawDebug)
	{
		FColor DrawColor = Collision ? FColor::Green : FColor::Red;

		DrawDebugSphere(GetWorld(), Origin, Radius, 26, DrawColor, false, 1.f);
	}
#endif
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
			AnimInstance->PlayMontage(TEXT("Attack"), FMath::RandRange(0, 1) ? TEXT("1") : TEXT("2"));
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

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
		{
			Deactivate();
		}, 3.f, false);
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