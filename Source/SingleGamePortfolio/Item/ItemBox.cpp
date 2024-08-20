// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemBox.h"
#include "Components/CapsuleComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Field/FieldSystemActor.h"
#include "Item/Item.h"

// Sets default values
AItemBox::AItemBox()
{
	PrimaryActorTick.bCanEverTick = false;
	
	mCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collider"));
	SetRootComponent(mCollider);
	mCollider->SetCollisionProfileName(TEXT("ItemBox"));
	mCollider->SetCapsuleHalfHeight(90.f);
	mCollider->SetCapsuleRadius(90.f);
	mCollider->SetCanEverAffectNavigation(false);

	mGeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("Mesh"));
	mGeometryCollection->SetupAttachment(mCollider);
	mGeometryCollection->SetCollisionProfileName(TEXT("Destructible"));
	mGeometryCollection->SetGenerateOverlapEvents(true);
	static ConstructorHelpers::FObjectFinder<UGeometryCollection>
		GC(TEXT("/Script/GeometryCollectionEngine.GeometryCollection'/Game/_ART/Props/Kobo_Dungeon/Meshes/BreakableMeshes/GC_SM-Pottery-04.GC_SM-Pottery-04'"));
	if (GC.Succeeded())
	{
		mGeometryCollection->SetRestCollection(GC.Object);
	}
	mGeometryCollection->SetRelativeScale3D(FVector(3.f));
	mGeometryCollection->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	mGeometryCollection->SetRenderCustomDepth(true);
}

// Called when the game starts or when spawned
void AItemBox::BeginPlay()
{
	Super::BeginPlay();

}

void AItemBox::GetHit_Implementation(const FVector& ImpactPoint)
{
	mCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//FTimerHandle TimerHandle;
	//GetWorld()->GetTimerManager().SetTimer(TimerHandle,
	//	[this]()
	//	{
	//		Destroy();
	//	}, 3.f, false);
	SpawnItem(GetActorLocation());
}

void AItemBox::SpawnItem(const FVector& SpawnLoc)
{
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (IsValid(mItemClass))
	{
		AItem* Item = GetWorld()->SpawnActor<AItem>
			(mItemClass, GetActorTransform(), Params);
		if (Item)
		{
			// TO DO..

		}
	}
}
