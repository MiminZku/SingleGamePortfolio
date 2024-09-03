// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyGameInstance.h"
#include "PlayerAnimTemplate.generated.h"

class APlayerCharacter;

UENUM(BlueprintType)
enum class EPlayerState: uint8
{
	UnArmed,
	Armed,
	Dead,
};

UCLASS()
class SINGLEGAMEPORTFOLIO_API UPlayerAnimTemplate : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPlayerAnimTemplate();


public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	//virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds);
	//virtual void NativePostEvaluateAnimation();
	//virtual void NativeUninitializeAnimation();

public:
	void ChangeState(EPlayerState State)
	{
		mCurrentState = State;
	}
	void SetAnimData(const FName& Name);
	void PlayMontage(const FName& Name, const FName& SectionName);
	FName GetNextAttackSection(const FName& CurrnetAttackName, bool IsWeak);

	void SetState(EPlayerState State) { mCurrentState = State; }

private:
	UFUNCTION()
	void MontageEnd(UAnimMontage* Montage, bool bInterrupted);

private:
	UFUNCTION()
	void AnimNotify_GrabWeapon();

	UFUNCTION()
	void AnimNotify_Holster();

	UFUNCTION()
	void AnimNotify_ComboEnable();

	UFUNCTION()
	void AnimNotify_ComboDisable();

	UFUNCTION()
	void AnimNotify_ComboEnd();

	UFUNCTION()
	void AnimNotify_Jump();

	UFUNCTION()
	void AnimNotify_Walk();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	APlayerCharacter* mOwningCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<FName, UAnimSequence*> mSequenceMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<FName, UBlendSpace*> mBlendSpaceMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<FName, UAnimMontage*> mMontageMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<FName, FNextAttack> mComboMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EPlayerState mCurrentState = EPlayerState::UnArmed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector mVelocity = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float mMoveSpeed = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bShouldMove = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsFalling = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bCanJump = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float mWalkForward = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float mWalkRight = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsRotating = false;
};
