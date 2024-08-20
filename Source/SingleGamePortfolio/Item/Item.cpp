// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/SphereComponent.h"
#include "Character/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	mCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));
	SetRootComponent(mCollider);
	mCollider->SetCollisionProfileName(TEXT("Item"));
	mCollider->SetSphereRadius(100.f);

	mMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mMesh->SetupAttachment(mCollider);
	mMesh->SetCollisionProfileName(TEXT("NoCollision"));
}

void AItem::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	mCollider->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnOverlapBegin);
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

}

void AItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if (Player)
	{
		if (IsValid(mPickupSound))
		{
			UGameplayStatics::PlaySoundAtLocation(
				this,
				mPickupSound,
				GetActorLocation()
			);
		}
		Destroy();
	}
}

