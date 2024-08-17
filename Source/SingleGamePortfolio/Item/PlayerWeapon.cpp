// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWeapon.h"
#include "../Character/PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"

APlayerWeapon::APlayerWeapon()
{

	mCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));
	SetRootComponent(mCollider);
	mCollider->SetCollisionProfileName(TEXT("Item"));
	mCollider->SetSphereRadius(100.f);

	mMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	mMesh->SetupAttachment(mCollider);
	mMesh->SetCollisionProfileName(TEXT("NoCollision"));

	mCollisionStartPos = CreateDefaultSubobject<USceneComponent>(TEXT("CollisionStartPos"));
	mCollisionStartPos->SetupAttachment(mMesh);

	mCollisionEndPos = CreateDefaultSubobject<USceneComponent>(TEXT("CollisionEndPos"));
	mCollisionEndPos->SetupAttachment(mMesh);

	mCollisionRadius = CreateDefaultSubobject<USceneComponent>(TEXT("CollisionRadius"));
	mCollisionRadius->SetupAttachment(mMesh);
}

void APlayerWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	mCollider->OnComponentBeginOverlap.AddDynamic(this, &APlayerWeapon::OnPlayerOverlap);
}

void APlayerWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerWeapon::OnPlayerOverlap(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if (Player)
	{
		if (!Player->HasWeapon())
		{
			mCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			mMesh->SetRelativeLocation(FVector::ZeroVector);
			mMesh->SetRelativeRotation(FRotator::ZeroRotator);
			mMesh->SetRelativeScale3D(FVector::OneVector);
			Player->PickWeaponUp(this);
		}
	}
}