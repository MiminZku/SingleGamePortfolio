// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/MonsterSpawner.h"
#include "Components/SphereComponent.h"
#include "Character/Enemy/MonsterBase.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Character/PlayerCharacter.h"
#include "Character/Enemy/MonsterPool.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AMonsterSpawner::AMonsterSpawner()
{
	mCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));
	mCollider->SetCollisionProfileName(TEXT("Item"));
	mCollider->bHiddenInGame = false;
	SetRootComponent(mCollider);

	mMonsterClass = AMonsterBase::StaticClass();
}

// Called when the game starts or when spawned
void AMonsterSpawner::BeginPlay()
{
	Super::BeginPlay();

	mCollider->SetSphereRadius(mSpawnRadius * 3);
	mCollider->OnComponentBeginOverlap.AddDynamic(this, &AMonsterSpawner::TriggerBeginOverlap);
	mCollider->OnComponentEndOverlap.AddDynamic(this, &AMonsterSpawner::TriggerEndOverlap);

	mMonsterPool = GetWorld()->SpawnActor<AMonsterPool>(AMonsterPool::StaticClass());
	if (mMonsterPool)
	{
		FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, true);
		mMonsterPool->AttachToActor(this, Rules);
		mMonsterPool->Init(this);
	}
}

void AMonsterSpawner::DetectedTarget(TObjectPtr<APawn> Target)
{
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&, Target]()
		{
			OnDetectTarget.Broadcast(Target);
		}, 1.f, false);
}

FVector AMonsterSpawner::GetRandomSpawnLoc()
{
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	FVector Origin = GetActorLocation();
	FNavLocation RandomPos;

	if (NavSystem->GetRandomPointInNavigableRadius(Origin, mSpawnRadius, OUT RandomPos))
	{
		return RandomPos.Location;
	}
	return Origin;
}

void AMonsterSpawner::TriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<APlayerCharacter>(OtherActor))
	{
		SpawnMonsters();
		OnTargetOverlap.Broadcast(Cast<APawn>(OtherActor));
	}
}

void AMonsterSpawner::TriggerEndOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<APlayerCharacter>(OtherActor))
	{
		DespawnMonsters();
	}
}

void AMonsterSpawner::SpawnMonsters()
{
	if (!IsValid(mMonsterClass))	return;

	int32 MonsterNum = FMath::RandRange(mMonsterMinNum, mMonsterMaxNum);

	for (int i = 0; i < MonsterNum; ++i)
	{
		AMonsterBase* SpawnMonster = mMonsterPool->GetMonster();
		if (SpawnMonster)
		{
			float RandomYaw = FMath::RandRange(0.f, 360.f);
			FVector SpawnLoc = GetRandomSpawnLoc();
			SpawnLoc += 
				FVector(0.f, 0.f, SpawnMonster->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
			FTransform SpawnTransform;
			SpawnTransform.SetLocation(SpawnLoc);
			SpawnTransform.SetRotation(FRotator(0.f, RandomYaw, 0.f).Quaternion());
			mMonsterArray.Add(SpawnMonster);
			SpawnMonster->SetActorTransform(SpawnTransform);
		}
	}
}

void AMonsterSpawner::DespawnMonsters()
{
	for (auto& Monster : mMonsterArray)
	{
		Monster->SetActorTransform(GetActorTransform());
		mMonsterPool->ReturnMonster(Monster);
	}
	mMonsterArray.Empty();
}