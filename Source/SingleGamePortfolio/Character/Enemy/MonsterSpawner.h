// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MonsterSpawner.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnDetectTargetDelegate, APawn*);

UCLASS()
class SINGLEGAMEPORTFOLIO_API AMonsterSpawner : public AActor
{
friend class AMonsterPool;

	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AMonsterSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	void DetectedTarget(TObjectPtr<APawn> Target);

private:
	UFUNCTION()
	void TriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void TriggerEndOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	void SpawnMonsters();
	void DespawnMonsters();

	FVector GetRandomSpawnLoc();


public:
	FOnDetectTargetDelegate OnDetectTarget;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USphereComponent> mTrigger;

	UPROPERTY()
	TObjectPtr<class AMonsterPool> mMonsterPool;

	UPROPERTY(EditAnywhere)
	float mSpawnRadius = 1000.f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AMonsterBase> mMonsterClass;

	UPROPERTY(EditAnywhere)
	int32 mMonsterMinNum = 6;
	
	UPROPERTY(EditAnywhere)
	int32 mMonsterMaxNum = 8;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<class AMonsterBase>> mMonsterArray;

};
