// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/LittleGoblin.h"

ALittleGoblin::ALittleGoblin()
{

	static ConstructorHelpers::FClassFinder<UAnimInstance>
		AnimClass(TEXT("/Game/_Programming/Character/Enemy/LittleGoblin/ABP_LittleGoblin.ABP_LittleGoblin_C"));
	if (AnimClass.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimClass.Class);
	}
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

void ALittleGoblin::Attack()
{
	Super::Attack();
	UMonsterAnimTemplate* AnimInstance = Cast<UMonsterAnimTemplate>(mAnimInstance);
	if (AnimInstance)
	{
		AnimInstance->PlayMontage(TEXT("Attack"), TEXT("Default"));
	}
}
