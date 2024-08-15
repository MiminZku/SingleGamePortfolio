// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBox.generated.h"

UCLASS()
class SINGLEGAMEPORTFOLIO_API AItemBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USphereComponent> mTrigger;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> mMesh;

};
