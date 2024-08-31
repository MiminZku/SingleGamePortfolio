// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SkeletonArrow.generated.h"

UCLASS()
class SINGLEGAMEPORTFOLIO_API ASkeletonArrow : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASkeletonArrow();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	void SetOwner(class AMonsterBase* Monster) { mOwner = Monster; }

	void Launch();

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> mArrowMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USphereComponent> mCollider;

	UPROPERTY()
	TObjectPtr<class AMonsterBase> mOwner;
};
