// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/SkeletonArrow.h"
#include "Components/SphereComponent.h"
#include "Interface/HitInterface.h"
#include "Engine/DamageEvents.h"
#include "MonsterBase.h"
#include "CharacterStat/CharacterStatComponent.h"

// Sets default values
ASkeletonArrow::ASkeletonArrow()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	mArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arrow"));
	SetRootComponent(mArrowMesh);
	mArrowMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	mArrowMesh->SetCollisionProfileName(TEXT("PhysicsActor"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		SM(TEXT("/Script/Engine.StaticMesh'/Game/_ART/Character/UndeadPack/SkeletonEnemy/Mesh/Weapon/Bow/Arrow/SM_Arrow.SM_Arrow'"));
	if (SM.Succeeded())
	{
		mArrowMesh->SetStaticMesh(SM.Object);
	}

	mCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider"));
	mCollider->SetupAttachment(mArrowMesh);
	mCollider->SetCollisionProfileName(TEXT("DetectPawn"));
	mCollider->SetSphereRadius(10.f);
	mCollider->SetRelativeLocation(FVector(35.f, 0.f, 0.f));

}

// Called when the game starts or when spawned
void ASkeletonArrow::BeginPlay()
{
	Super::BeginPlay();
	
	mCollider->OnComponentBeginOverlap.AddDynamic(this, &ASkeletonArrow::OnHit);
}

void ASkeletonArrow::OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IHitInterface* HitActor = Cast<IHitInterface>(SweepResult.GetActor());
	if (HitActor)
	{
		if (!HitActor->IsDamaged())
		{
			HitActor->Execute_GetHit(OtherActor, mCollider->GetComponentLocation());

			FDamageEvent DmgEvent;
			OtherActor->TakeDamage(mOwner->GetStatComponent()->GetAtk(), DmgEvent, mOwner->GetController(), this);

	#if ENABLE_DRAW_DEBUG
			DrawDebugSphere(GetWorld(), mCollider->GetComponentLocation(), mCollider->GetScaledSphereRadius(), 8,
					FColor::Green, false, 1.f);
	#endif
		}
	}

	//Destroy();
	SetActorHiddenInGame(true);
	mCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASkeletonArrow::Launch()
{
	mCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	mArrowMesh->SetSimulatePhysics(true);
	mArrowMesh->AddImpulse(GetActorForwardVector() * 1500.f, NAME_None, true);
	//SetLifeSpan(3.f);
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle,
		[&]()
		{
			SetActorHiddenInGame(true);
			mCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}, 3.f, false);
}
