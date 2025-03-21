// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Enemy/MonsterBase.h"
#include "SkeletonMonster.generated.h"

/**
 * 
 */
UCLASS()
class SINGLEGAMEPORTFOLIO_API ASkeletonMonster : public AMonsterBase
{
	GENERATED_BODY()

public:
	ASkeletonMonster();

protected:
	virtual void PostInitializeComponents() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void AttackCollisionCheckOnce(EAttackType AttackType, FVector Offset, float Radius, float Coefficient) override;

	virtual void Angry() override;
	virtual void Attack() override;

	virtual void Activate() override;

protected:
	void ReloadArrow();
	void LaunchArrow();

protected:
	UPROPERTY()
	TObjectPtr<USkeletalMeshComponent> mBowMesh;

	UPROPERTY()
	TObjectPtr<UAnimMontage> mBowAnimMontage;

	UPROPERTY()
	TSubclassOf<class ASkeletonArrow> mArrowClass;

	UPROPERTY()
	TArray<TObjectPtr<class ASkeletonArrow>> mArrowPool;

	uint8 mCurrentArrowIdx = 0;
};
