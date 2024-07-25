// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerDefaultAnimTemplate.generated.h"

UENUM(BlueprintType)
enum class EPlayerState: uint8
{
	UnArmed,
	Armed,
	Dead,
};

UCLASS()
class SINGLEGAMEPORTFOLIO_API UPlayerDefaultAnimTemplate : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPlayerDefaultAnimTemplate();


public:
	virtual void NativeInitializeAnimation();
	virtual void NativeUpdateAnimation(float DeltaSeconds);
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds);
	virtual void NativePostEvaluateAnimation();
	virtual void NativeUninitializeAnimation();
	virtual void NativeBeginPlay();

public:
	void ChangeState(EPlayerState State)
	{
		mCurrentState = State;
	}
	void SetAnimData(const FName& Name);
	void PlayMontage(const FString& Name);

	void SetState(EPlayerState State) { mCurrentState = State; }
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FString, UAnimSequence*> mSequenceMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FString, UBlendSpace*> mBlendSpaceMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FString, UAnimMontage*> mMontageMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EPlayerState mCurrentState = EPlayerState::UnArmed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector mVelocity = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float mMoveSpeed = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bShouldMove = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsFalling = false;


};
