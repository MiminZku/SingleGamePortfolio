// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Enemy/MonsterBase.h"
#include "StelaeKnight.generated.h"

/**
 * 
 */
UCLASS()
class SINGLEGAMEPORTFOLIO_API AStelaeKnight : public AMonsterBase
{
	GENERATED_BODY()
	
public:
	AStelaeKnight();

protected:
	virtual void PostInitializeComponents() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount,
		struct FDamageEvent const& DamageEvent, class AController* EventInstigator,
		AActor* DamageCauser) override;

	virtual void Attack() override;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> mSword;

private:

};
