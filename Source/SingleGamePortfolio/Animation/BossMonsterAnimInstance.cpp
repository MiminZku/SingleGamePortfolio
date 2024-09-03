// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/BossMonsterAnimInstance.h"
#include "Character/Enemy/MonsterBase.h"

UBossMonsterAnimInstance::UBossMonsterAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		AttackMontage(TEXT("/Script/Engine.AnimMontage'/Game/_Programming/Character/Enemy/StelaeKnight/AM_StelaeKnight_Attack.AM_StelaeKnight_Attack'"));
	if (AttackMontage.Succeeded())
	{
		mAttackMontage = AttackMontage.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		HitMontage(TEXT("/Script/Engine.AnimMontage'/Game/_Programming/Character/Enemy/StelaeKnight/AM_StelaeKnight_Hit.AM_StelaeKnight_Hit'"));
	if (HitMontage.Succeeded())
	{
		mHitMontage = HitMontage.Object;
	}
}

void UBossMonsterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	mOwningCharacter = Cast<AMonsterBase>(TryGetPawnOwner());
}

void UBossMonsterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OnMontageEnded.AddDynamic(this, &UBossMonsterAnimInstance::MontageEnd);
}

void UBossMonsterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (mOwningCharacter)
	{
		FVector Velocity = mOwningCharacter->GetVelocity();
		float MoveSpeed = Velocity.Length();

		float Direction = CalculateDirection(Velocity, mOwningCharacter->GetActorRotation());
		mWalkForward = MoveSpeed * FMath::Cos(FMath::DegreesToRadians(Direction));
		mWalkRight = MoveSpeed * FMath::Sin(FMath::DegreesToRadians(Direction));
	}
}

void UBossMonsterAnimInstance::PlayMontage(const FName& Name, const FName& SectionName)
{
	UAnimMontage* Montage = nullptr;
	if (TEXT("Attack") == Name) Montage = mAttackMontage;
	else if (TEXT("Hit") == Name) Montage = mHitMontage;

	if (Montage)
	{
		Montage_Play(Montage);
		Montage_JumpToSection(SectionName);
	}
}

void UBossMonsterAnimInstance::MontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	if (!IsValid(mOwningCharacter))	return;
	if (Montage == mAttackMontage)
	{
		mOwningCharacter->OnAttackFinished.ExecuteIfBound();

		return;
	}
	if (Montage == mHitMontage)
	{

		return;
	}
}
