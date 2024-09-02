// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/StelaeKnight.h"
#include "Components/CapsuleComponent.h"
#include "AI/BossMonsterController.h"
#include "CharacterStat/CharacterStatComponent.h"

AStelaeKnight::AStelaeKnight()
{
	AIControllerClass = ABossMonsterController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SKM(TEXT("/Script/Engine.SkeletalMesh'/Game/_ART/Character/StelaeKnight/StelaeKnight.StelaeKnight'"));
	if (SKM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SKM.Object);
	}
	static ConstructorHelpers::FClassFinder<UAnimInstance>
		AnimClass(TEXT("/Game/_Programming/Character/Enemy/StelaeKnight/ABP_StelaeKnight.ABP_StelaeKnight_C"));
	if (AnimClass.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimClass.Class);
	}
	GetMesh()->SetRelativeLocation(FVector(-45.f, 0.f, -120.f));

	GetCapsuleComponent()->SetCapsuleHalfHeight(120.f);

	mSword = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sword"));
	mSword->SetupAttachment(GetMesh(), TEXT("Sword"));
	mSword->SetCanEverAffectNavigation(false);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		SM(TEXT("/Script/Engine.StaticMesh'/Game/_ART/Character/UndeadPack/SkeletonEnemy/Mesh/Weapon/Sword/SM_Sword.SM_Sword'"));
	if (SM.Succeeded())
	{
		mSword->SetStaticMesh(SM.Object);
	}

}

void AStelaeKnight::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	mStats->SetStats(5);
}

float AStelaeKnight::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);



	return Damage;
}