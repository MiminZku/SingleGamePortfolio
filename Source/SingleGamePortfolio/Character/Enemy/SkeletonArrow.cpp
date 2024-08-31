// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/SkeletonArrow.h"

// Sets default values
ASkeletonArrow::ASkeletonArrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASkeletonArrow::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASkeletonArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

