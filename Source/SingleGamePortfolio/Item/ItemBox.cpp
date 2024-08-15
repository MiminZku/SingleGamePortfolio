// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemBox.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AItemBox::AItemBox()
{
	mTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));
	mMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	
	SetRootComponent(mTrigger);
	mMesh->SetupAttachment(mTrigger);

	mTrigger->SetCollisionProfileName(TEXT("Item"));

	mMesh->SetCollisionProfileName(TEXT("NoCollision"));

}

// Called when the game starts or when spawned
void AItemBox::BeginPlay()
{
	Super::BeginPlay();
	
}
