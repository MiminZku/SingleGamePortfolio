// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/HitInterface.h"
#include "ItemBox.generated.h"

UCLASS()
class SINGLEGAMEPORTFOLIO_API AItemBox : public AActor, public IHitInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

	void SpawnItem(const FVector& SpawnLoc);

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UCapsuleComponent> mCollider;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UGeometryCollectionComponent> mGeometryCollection;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AItem> mItemClass;
};
