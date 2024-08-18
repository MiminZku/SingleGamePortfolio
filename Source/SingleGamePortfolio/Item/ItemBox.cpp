// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemBox.h"
#include "Components/BoxComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

// Sets default values
AItemBox::AItemBox()
{
	PrimaryActorTick.bCanEverTick = false;
	
	mCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	SetRootComponent(mCollider);
	mCollider->SetCollisionProfileName(TEXT("ItemBox"));
	mCollider->SetBoxExtent(FVector(100.f));
	mCollider->SetCanEverAffectNavigation(false);

	mGeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("Mesh"));
	mGeometryCollection->SetupAttachment(mCollider);
	mGeometryCollection->SetGenerateOverlapEvents(true);
	static ConstructorHelpers::FObjectFinder<UGeometryCollection>
		GC(TEXT("/Script/GeometryCollectionEngine.GeometryCollection'/Game/_ART/Props/Kobo_Dungeon/Meshes/BreakableMeshes/GC_SM-Pottery-04.GC_SM-Pottery-04'"));
	if (GC.Succeeded())
	{
		mGeometryCollection->SetRestCollection(GC.Object);
	}
	mGeometryCollection->SetRelativeScale3D(FVector(3.f));
	mGeometryCollection->SetRelativeLocation(FVector(0.f, 0.f, -100.f));
}

// Called when the game starts or when spawned
void AItemBox::BeginPlay()
{
	Super::BeginPlay();

	mCollider->OnComponentBeginOverlap.AddDynamic(this, &AItemBox::OnOverlapBegin);
}

void AItemBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, TEXT("BoxOverlap"));
	mCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AItemBox::SetColliderEnable(bool Enable)
{
	if (false)
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, TEXT("SetColliderEnable"));
		mCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}
