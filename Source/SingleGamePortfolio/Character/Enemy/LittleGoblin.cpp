// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/LittleGoblin.h"
#include "Components/CapsuleComponent.h"

ALittleGoblin::ALittleGoblin()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SKM(TEXT("/Script/Engine.SkeletalMesh'/Game/_ART/Character/UndeadPack/EnemyGoblin/Mesh/SM_EnemyGoblin.SM_EnemyGoblin'"));
	if (SKM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SKM.Object);
	}
	static ConstructorHelpers::FClassFinder<UAnimInstance>
		AnimClass(TEXT("/Game/_Programming/Character/Enemy/LittleGoblin/ABP_LittleGoblin.ABP_LittleGoblin_C"));
	if (AnimClass.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimClass.Class);
	}
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -80.f));
	GetMesh()->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5f));

	GetCapsuleComponent()->SetCapsuleHalfHeight(80.f);
	GetCapsuleComponent()->SetCapsuleRadius(55.f);
}

void ALittleGoblin::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	mAnimInstance = GetMesh()->GetAnimInstance();
	UMonsterAnimTemplate* AnimInstance = Cast<UMonsterAnimTemplate>(mAnimInstance);
	if (AnimInstance)
	{
		AnimInstance->SetAnimData(TEXT("LittleGoblin"));
	}
}

void ALittleGoblin::BeginPlay()
{
	Super::BeginPlay();

}

void ALittleGoblin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALittleGoblin::Angry()
{
	Super::Angry();
}

void ALittleGoblin::Attack()
{
	Super::Attack();
}
