// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWeapon.h"
#include "../BasicCharacter.h"
#include "Components/SphereComponent.h"

APlayerWeapon::APlayerWeapon()
{

	mTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));
	SetRootComponent(mTrigger);
	mTrigger->SetCollisionProfileName(TEXT("Item"));
	mTrigger->SetSphereRadius(100.f);

	mMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	mMesh->SetupAttachment(mTrigger);
	mMesh->SetCollisionProfileName(TEXT("NoCollision"));
}

void APlayerWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	mTrigger->OnComponentBeginOverlap.AddDynamic(this, &APlayerWeapon::OnPlayerOverlap);
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
	ABasicCharacter* Player = Cast<ABasicCharacter>(OtherActor);
	if (Player)
	{
		if (!Player->HasWeapon())
		{
			mTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			mMesh->SetRelativeLocation(FVector::ZeroVector);
			mMesh->SetRelativeRotation(FRotator::ZeroRotator);
			mMesh->SetRelativeScale3D(FVector::OneVector);
			Player->PickWeaponUp(this);
		}
	}
}
