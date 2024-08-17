// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerWeapon.generated.h"

class USphereComponent;

UCLASS()
class SINGLEGAMEPORTFOLIO_API APlayerWeapon : public AActor
{
	GENERATED_BODY()

public:
	APlayerWeapon();

protected:
	virtual void PostInitializeComponents();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector& FieldLocation);

protected:
	UFUNCTION()
	virtual void OnPlayerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

public:
	UStaticMeshComponent* GetMesh() { return mMesh; }

	FVector GetCollisionStartPos() { return mCollisionStartPos->GetComponentLocation(); }
	FVector GetCollisonEndPos() { return mCollisionEndPos->GetComponentLocation();}
	FVector GetCollisionRadiusPos()	{ return mCollisionRadius->GetComponentLocation();}

	FVector GetPrevCollisionPos() { return mPrevCollisionPos; }

	void SetPrevCollisionPos(const FVector& Vec) {	mPrevCollisionPos = Vec; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* mMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* mCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* mCollisionStartPos = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* mCollisionEndPos = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* mCollisionRadius = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector mPrevCollisionPos = FVector::ZeroVector;
};