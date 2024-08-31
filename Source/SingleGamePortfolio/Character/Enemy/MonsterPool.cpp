// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/MonsterPool.h"
#include "Character/Enemy/MonsterSpawner.h"
#include "Character/Enemy/MonsterBase.h"
#include "NavigationSystem.h"

// Sets default values
AMonsterPool::AMonsterPool()
{
	mRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(mRoot);

}

// Called when the game starts or when spawned
void AMonsterPool::BeginPlay()
{
	Super::BeginPlay();
}

void AMonsterPool::Init(TObjectPtr<AMonsterSpawner> MonsterSpawner)
{
	mMonsterClass = MonsterSpawner->mMonsterClass;
	mMonsterNum = MonsterSpawner->mMonsterMaxNum;
	SpawnMonsters(MonsterSpawner);
}

void AMonsterPool::SpawnMonsters(TObjectPtr<AMonsterSpawner> MonsterSpawner)
{
	for (int i = 0; i < mMonsterNum; ++i)
	{
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		if (IsValid(mMonsterClass))
		{
			AMonsterBase* SpawnMonster = GetWorld()->SpawnActor<AMonsterBase>
				(mMonsterClass, GetActorTransform(), Params);
			if (SpawnMonster)
			{
				mMonsterArray.Add(SpawnMonster);
				SpawnMonster->BindSpawner(MonsterSpawner);
				SpawnMonster->Deactivate();
			}
		}
	}
	mTop = mMonsterArray.Num();
}

TObjectPtr<class AMonsterBase> AMonsterPool::GetMonster()
{
	if (mTop == 0)	return nullptr;
	
	AMonsterBase* Monster = mMonsterArray[--mTop];
	Monster->Activate();

	return Monster;
}

void AMonsterPool::ReturnMonster(TObjectPtr<class AMonsterBase> Monster)
{
	Monster->Deactivate();
	mMonsterArray[mTop++] = Monster;
}
