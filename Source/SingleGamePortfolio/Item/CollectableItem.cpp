// Fill out your copyright notice in the Description page of Project Settings.


#include "CollectableItem.h"
#include "Components/SphereComponent.h"
#include "CollectableItem.h"

// Sets default values
ACollectableItem::ACollectableItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));
	SetRootComponent(mCollider);
	mCollider->SetCollisionProfileName(TEXT("Item"));
	mCollider->SetSphereRadius(100.f);

	mMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mMesh->SetupAttachment(mCollider);
	mMesh->SetCollisionProfileName(TEXT("NoCollision"));
}

void ACollectableItem::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	mCollider->OnComponentBeginOverlap.AddDynamic(this, &ACollectableItem::OnPlayerOverlap);
}

// Called when the game starts or when spawned
void ACollectableItem::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACollectableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACollectableItem::OnPlayerOverlap(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{

}

