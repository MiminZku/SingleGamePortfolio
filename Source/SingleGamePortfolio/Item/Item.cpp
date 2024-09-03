// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/SphereComponent.h"
#include "Character/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "CharacterStat/CharacterStatComponent.h"

// Sets default values
AItem::AItem()
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
	mMesh->SetRenderCustomDepth(true);
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

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	mTime += DeltaTime;

	if (bHovering)
	{
		mMesh->SetRelativeLocation(FVector(0.f, 0.f, 20 * FMath::Sin(mTime * 3.f)));
		AddActorLocalRotation(FRotator(0.f, DeltaTime * 50.f, 0.f));
	}
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
		if (bIsHpPotion)	Player->GetStatComponent()->RecoverHp(50.f);
		else				Player->GetStatComponent()->RecoverMp(50.f);
		Destroy();
	}
}

