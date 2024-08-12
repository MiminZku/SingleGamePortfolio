// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/MonsterSpawner.h"
#include "Components/SphereComponent.h"
#include "Character/Enemy/MonsterBase.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMonsterSpawner::AMonsterSpawner()
{
	mTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));
	mTrigger->SetCollisionProfileName(TEXT("Item"));
	mTrigger->bHiddenInGame = false;
	SetRootComponent(mTrigger);

	mMonsterClass = AMonsterBase::StaticClass();

}



// Called when the game starts or when spawned
void AMonsterSpawner::BeginPlay()
{
	Super::BeginPlay();

	mTrigger->SetSphereRadius(mSpawnRadius * 2);
	mTrigger->OnComponentBeginOverlap.AddDynamic(this,
		&AMonsterSpawner::TriggerBeginOverlap);
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
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue,
		TEXT("OnTriggerBegin"));
	SpawnMonsters();
}

void AMonsterSpawner::SpawnMonsters()
{
	int32 MonsterNum = FMath::RandRange(mMonsterMinNum, mMonsterMaxNum);

	for (int i = 0; i < MonsterNum; ++i)
	{
		float RandomYaw = FMath::RandRange(0.f, 360.f);

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		FVector SpawnLoc = GetRandomSpawnLoc();

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SpawnLoc);
		SpawnTransform.SetRotation(FRotator(0.f, RandomYaw, 0.f).Quaternion());
		if (IsValid(mMonsterClass))
		{
			AMonsterBase* SpawnMonster = GetWorld()->SpawnActor<AMonsterBase>
				(mMonsterClass, SpawnTransform, Params);
			if (SpawnMonster)
			{
				mMonsterArray.Add(SpawnMonster);
				SpawnMonster->BindSpawner(this);
			}

			//AMonsterBase* SpawnMonster = GetWorld()->SpawnActorDeferred<AMonsterBase>(
			//	mMonsterClass, SpawnTransform, nullptr, nullptr, 
			//	ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
			//if (SpawnMonster)
			//{
			//	mMonsterArray.Add(SpawnMonster);
			//	SpawnMonster->BindSpawner(this);
			//	UGameplayStatics::FinishSpawningActor(SpawnMonster, SpawnTransform);
			//}
		}
	}
}

