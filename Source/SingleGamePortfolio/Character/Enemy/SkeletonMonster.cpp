// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/SkeletonMonster.h"
#include "Components/CapsuleComponent.h"
#include "CharacterStat/CharacterStatComponent.h"

ASkeletonMonster::ASkeletonMonster()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SKM(TEXT("/Script/Engine.SkeletalMesh'/Game/_ART/Character/UndeadPack/SkeletonEnemy/Mesh/SK_Skeleton.SK_Skeleton'"));
	if (SKM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SKM.Object);
	}
	static ConstructorHelpers::FClassFinder<UAnimInstance>
		AnimClass(TEXT("/Game/_Programming/Character/Enemy/SkeletonEnemy/ABP_SkeletonEnemy.ABP_SkeletonEnemy_C"));
	if (AnimClass.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimClass.Class);
	}
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -85.f));

	mBowMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Bow"));
	mBowMesh->SetupAttachment(GetMesh(), TEXT("SKT_Bow"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		WeaponSKM(TEXT("/Script/Engine.SkeletalMesh'/Game/_ART/Character/UndeadPack/SkeletonEnemy/Mesh/Weapon/Bow/SK_Bow.SK_Bow'"));
	if (WeaponSKM.Succeeded())
	{
		mBowMesh->SetSkeletalMesh(WeaponSKM.Object);
	}
	static ConstructorHelpers::FClassFinder<UAnimInstance>
		WeaponAnimClass(TEXT("/Game/_Programming/Character/Enemy/SkeletonEnemy/ABP_Bow.ABP_Bow_C"));
	if (WeaponAnimClass.Succeeded())
	{
		mBowMesh->SetAnimInstanceClass(WeaponAnimClass.Class);
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		WeaponMontage(TEXT("/Script/Engine.AnimMontage'/Game/_Programming/Character/Enemy/SkeletonEnemy/AM_Bow.AM_Bow'"));
	if (WeaponMontage.Succeeded())
	{
		mBowAnimMontage = WeaponMontage.Object;
	}

	GetCapsuleComponent()->SetCapsuleHalfHeight(85.f);
	GetCapsuleComponent()->SetCapsuleRadius(25.f);
}

void ASkeletonMonster::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	mAnimInstance = GetMesh()->GetAnimInstance();
	UMonsterAnimTemplate* AnimInstance = Cast<UMonsterAnimTemplate>(mAnimInstance);
	if (AnimInstance)
	{
		AnimInstance->SetAnimData(TEXT("Skeleton"));
	}
	mStats->SetStats(2);
	
	SetAttackRange(800.f);
}

void ASkeletonMonster::BeginPlay()
{
	Super::BeginPlay();

}

void ASkeletonMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASkeletonMonster::Angry()
{
	Super::Angry();

}

void ASkeletonMonster::Attack()
{
	Super::Attack();
	mBowMesh->GetAnimInstance()->Montage_Play(mBowAnimMontage);


}
