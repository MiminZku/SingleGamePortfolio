// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class USphereComponent;

UCLASS()
class SINGLEGAMEPORTFOLIO_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	virtual void PostInitializeComponents();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:

	void SetHoveringEnable(bool Enable) { bHovering = Enable; }

protected:
	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* mCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* mMesh;

private:
	float mTime = 0.f;

	bool bHovering = true;

	UPROPERTY(EditAnywhere)
	USoundBase* mPickupSound;

	UPROPERTY(EditAnywhere)
	UParticleSystem* mPickupParticle;

	UPROPERTY(EditAnywhere)
	bool bIsHpPotion = true;
};
