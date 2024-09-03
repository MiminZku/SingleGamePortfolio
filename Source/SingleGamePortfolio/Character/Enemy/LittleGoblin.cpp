// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/LittleGoblin.h"
#include "Components/CapsuleComponent.h"
#include "CharacterStat/CharacterStatComponent.h"
#include "Engine/DamageEvents.h"

ALittleGoblin::ALittleGoblin()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SKM(TEXT("/Script/Engine.SkeletalMesh'/Game/_ART/Character/UndeadPack/EnemyGoblin/Mesh/SM_EnemyGoblin.SM_EnemyGoblin'"));
	if (SKM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SKM.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance>
		AnimClass(TEXT("/Game/_Programming/Character/Enemy/LittleGoblin/ABP_LittleGoblin.ABP_LittleGoblin_C"));
	if (AnimClass.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimClass.Class);
	}
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -80.f));
	GetMesh()->SetRelativeScale3D(FVector(1.3f, 1.3f, 1.3f));

	GetCapsuleComponent()->SetCapsuleHalfHeight(80.f);
	GetCapsuleComponent()->SetCapsuleRadius(55.f);
}

void ALittleGoblin::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UMonsterAnimTemplate* AnimInstance = Cast<UMonsterAnimTemplate>(mAnimInstance);
	if (AnimInstance)
	{
		AnimInstance->SetAnimData(TEXT("LittleGoblin"));
	}
	mStats->SetStats(1);
}

void ALittleGoblin::BeginPlay()
{
	Super::BeginPlay();

}

void ALittleGoblin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALittleGoblin::AttackCollisionCheckOnce(EAttackType AttackType, FVector Offset, float Radius, float Coefficient)
{
	Super::AttackCollisionCheckOnce(AttackType, Offset, Radius, Coefficient);

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
			IHitInterface* AttackedCharacter = Cast<IHitInterface>(HitResult.GetActor());
			if (AttackedCharacter)
			{
				if (!AttackedCharacter->IsDamaged())
				{
					AttackedCharacter->GetHit_Implementation(HitResult.ImpactPoint);

					FDamageEvent DmgEvent;
					HitResult.GetActor()->TakeDamage(mStats->GetAtk(), DmgEvent, GetController(), this);
				}
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

void ALittleGoblin::Angry()
{
	Super::Angry();
}

void ALittleGoblin::Attack()
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
