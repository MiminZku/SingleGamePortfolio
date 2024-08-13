// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MonsterPool.generated.h"

UCLASS()
class SINGLEGAMEPORTFOLIO_API AMonsterPool : public AActor
{
	GENERATED_BODY()


public:	
	// Sets default values for this actor's properties
	AMonsterPool();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void Init(TObjectPtr<class AMonsterSpawner> MonsterSpawner);

	void SpawnMonsters(TObjectPtr<class AMonsterSpawner> MonsterSpawner);

	TObjectPtr<class AMonsterBase> GetMonster();
	void ReturnMonster(TObjectPtr<class AMonsterBase> Monster);

private:

private:
	UPROPERTY()
	TObjectPtr<USceneComponent> mRoot;

	UPROPERTY()
	TSubclassOf<class AMonsterBase> mMonsterClass;

	UPROPERTY()
	TArray<TObjectPtr<class AMonsterBase>> mMonsterArray;

	int32 mMonsterNum = 10;

	int32 mTop = 0;
};
