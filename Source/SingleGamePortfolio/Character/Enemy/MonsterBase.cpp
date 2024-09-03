// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/MonsterBase.h"
#include "AI/MonsterController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Enemy/MonsterSpawner.h"
#include "CharacterStat/CharacterStatComponent.h"
#include "UI/ProgressBarWidget.h"
#include "Components/WidgetComponent.h"
#include "Character/PlayerCharacter.h"

AMonsterBase::AMonsterBase()
{
	AIControllerClass = AMonsterController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCharacterMovement()->MaxWalkSpeed = 200.f;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f);
	
	// 몬스터끼리 비켜가게
	//GetCharacterMovement()->bUseRVOAvoidance = true; 
	//GetCharacterMovement()->AvoidanceConsiderationRadius = GetCapsuleComponent()->GetScaledCapsuleRadius() * 3.f; 

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Monster"));
	
	GetMesh()->VisibilityBasedAnimTickOption = 
		EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;

	mTarget = nullptr;

	// Widget Component
	mHpBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HpBar"));
	mHpBarWidget->SetupAttachment(RootComponent);
	mHpBarWidget->SetRelativeLocation(FVector(0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));

	const static ConstructorHelpers::FClassFinder<UUserWidget>
		HpBar(TEXT("/Game/_Programming/UI/WBP_HpBar.WBP_HpBar_C"));
	if (HpBar.Class)
	{
		mHpBarWidget->SetWidgetClass(HpBar.Class);
		mHpBarWidget->SetDrawSize(FVector2D(150.f, 15.f));
		mHpBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
		mHpBarWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AMonsterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	mAnimInstance = GetMesh()->GetAnimInstance();

	mHpBarWidget->InitWidget();
	UProgressBarWidget* HpBar = Cast<UProgressBarWidget>(mHpBarWidget->GetUserWidgetObject());
	if (HpBar)
	{
		HpBar->BindStat(mStats, TEXT("Hp"));
		HpBar->UpdateProgressBar();
		mHpBarWidget->SetHiddenInGame(true);
	}
}

float AMonsterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	mHpBarWidget->SetHiddenInGame(false);

	if(IsValid(mSpawner)) DetectedTarget(EventInstigator->GetPawn());

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

void AMonsterBase::AttackCollisionCheckOnce(EAttackType AttackType, FVector Offset, float Radius, float Coefficient)
{

}

void AMonsterBase::GetHit_Implementation(const FVector& ImpactPoint)
{
	FVector LaunchVec = GetActorLocation() - ImpactPoint;
	float Dist = LaunchVec.Length();
	Dist = 100 / Dist;
	LaunchCharacter(LaunchVec * 10.f * Dist, false, false);
}

void AMonsterBase::Activate()
{
	RegisterTarget(nullptr);
	SetState(EMonsterState::Patrol);
	SetActorHiddenInGame(false);
	mHpBarWidget->SetHiddenInGame(true);
	SetActorTickEnabled(true);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Monster"));
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCharacterMovement()->bUseRVOAvoidance = true;
	AMonsterController* Ctrl = Cast<AMonsterController>(GetController());
	if (Ctrl)
	{
		Ctrl->RunAI();
	}
	mStats->RecoverHp(mStats->GetMaxStat(TEXT("Hp")));
}

void AMonsterBase::Deactivate()
{
	SetActorHiddenInGame(true);
	if(IsValid(mSpawner)) SetActorLocation(mSpawner->GetActorLocation());
	SetActorTickEnabled(false);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->bUseRVOAvoidance = false;
	AMonsterController* Ctrl = Cast<AMonsterController>(GetController());
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
			AnimInstance->PlayMontage(TEXT("Attack"), TEXT("Attack"));
		}
	}
}

void AMonsterBase::SetState(EMonsterState InState)
{
	if (bIsBoss) return;

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
		GetCharacterMovement()->MaxWalkSpeed = 200.f;
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

	mHpBarWidget->SetHiddenInGame(true);

	AMonsterController* Ctrl = Cast<AMonsterController>(GetController());
	if (Ctrl)
	{
		Ctrl->StopAI();
	}
	if (mAnimInstance)
	{
		UMonsterAnimTemplate* AnimInstance = Cast<UMonsterAnimTemplate>(mAnimInstance);
		if (AnimInstance)
		{
			AnimInstance->StopAllMontages(0.1f);
			AnimInstance->SetDead(true);
		}
	}
	GetCharacterMovement()->bUseRVOAvoidance = false;

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
		{
			Deactivate();
		}, 3.f, false);

	APlayerCharacter* Player = Cast<APlayerCharacter>(mTarget);
	if (Player)
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan,
			FString::Printf(TEXT("+ %.0f exp"), mStats->GetDropExp()));
		Player->GetStatComponent()->ExpUp(mStats->GetDropExp());
		Player->LockOff();
	}
}

void AMonsterBase::BindSpawner(AMonsterSpawner* Spawner)
{
	mSpawner = Spawner;
	if (IsValid(mSpawner))
		mSpawner->OnDetectTarget.AddUObject(this, &AMonsterBase::RegisterTarget);

	if (bIsBoss)
	{
		mSpawner->OnTargetOverlap.AddUObject(this, &AMonsterBase::RegisterTarget);
	}	
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
	AMonsterController* Ctrl = Cast<AMonsterController>(GetController());
	if (Ctrl)
	{
		Ctrl->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), Target);
	}
}