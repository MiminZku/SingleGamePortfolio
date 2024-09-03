// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterBase.h"
#include "CharacterStat/CharacterStatComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->bReceivesDecals = false;
	GetMesh()->SetRenderCustomDepth(true);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	GetCharacterMovement()->bUseRVOAvoidance = true;
	GetCharacterMovement()->AvoidanceConsiderationRadius = GetCapsuleComponent()->GetScaledCapsuleRadius() * 3.f;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Stat Component
	mStats = CreateDefaultSubobject<UCharacterStatComponent>(TEXT("Stats"));

}  

void ACharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	mStats->OnHpZero.AddUObject(this, &ACharacterBase::Die);
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ACharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&, DamageAmount]()
		{
			mStats->ApplyDamage(DamageAmount);
		}, 0.1f, false);
	return Damage;
}

void ACharacterBase::SetCollisionEnable(bool Enable)
{
}

void ACharacterBase::Die()
{
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("IgnoreOnlyPawn"));
}
