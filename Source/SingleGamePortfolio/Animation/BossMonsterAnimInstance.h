// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BossMonsterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SINGLEGAMEPORTFOLIO_API UBossMonsterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UBossMonsterAnimInstance();

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	//virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds);
	//virtual void NativePostEvaluateAnimation();
	//virtual void NativeUninitializeAnimation();

	void SetDead(bool IsDead) { bIsDead = IsDead; }

	void PlayMontage(const FName& Name, const FName& SectionName);

private:
	UFUNCTION()
	void MontageEnd(UAnimMontage* Montage, bool bInterrupted);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class AMonsterBase* mOwningCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float mWalkForward = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float mWalkRight = 0.f;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly)
	bool bIsDead = false;	

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> mAttackMontage;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> mHitMontage;
};
