// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MonsterAnimTemplate.generated.h"

UENUM(BlueprintType)
enum class EMonsterState : uint8
{
	Patrol,
	Trace
};
/**
 * 
 */
UCLASS()
class SINGLEGAMEPORTFOLIO_API UMonsterAnimTemplate : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UMonsterAnimTemplate();

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	//virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds);
	//virtual void NativePostEvaluateAnimation();
	//virtual void NativeUninitializeAnimation();
	virtual void NativeBeginPlay() override;

	void SetAnimData(const FName& Name);

	void PlayMontage(const FName& Name, const FName& SectionName);

	void SetState(EMonsterState InState) { mCurrentState = InState; }

protected:


private:
	UFUNCTION()
	void MontageEnd(UAnimMontage* Montage, bool bInterrupted);


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class AMonsterBase* mOwningCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<FName, UAnimSequence*> mSequenceMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<FName, UBlendSpace*> mBlendSpaceMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<FName, UAnimMontage*> mMontageMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EMonsterState mCurrentState = EMonsterState::Patrol;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector mVelocity = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float mMoveSpeed = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bShouldMove = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsFalling = false;
};
