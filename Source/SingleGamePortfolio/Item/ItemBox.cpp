// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemBox.h"
#include "Components/CapsuleComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

// Sets default values
AItemBox::AItemBox()
{
	PrimaryActorTick.bCanEverTick = false;
	
	mGeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("Mesh"));
	//mGeometryCollection->SetCollisionProfileName(TEXT("ItemBox"));
	SetRootComponent(mGeometryCollection);
	mGeometryCollection->SetGenerateOverlapEvents(true);
	static ConstructorHelpers::FObjectFinder<UGeometryCollection>
		GC(TEXT("/Script/GeometryCollectionEngine.GeometryCollection'/Game/_ART/Props/Kobo_Dungeon/Meshes/SM-Pottery-03_SM-Pottery-03/BreakablePottery.BreakablePottery'"));
	if (GC.Succeeded())
	{
		mGeometryCollection->SetRestCollection(GC.Object);
	}

}

// Called when the game starts or when spawned
void AItemBox::BeginPlay()
{
	Super::BeginPlay();

	
}

//void AItemBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
//	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//
//}
