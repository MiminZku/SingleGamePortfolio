// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Interface/AttackInterface.h"
#include "AN_AttackCollision.generated.h"

/**
 * 
 */
UCLASS()
class SINGLEGAMEPORTFOLIO_API UAN_AttackCollision : public UAnimNotify
{
	GENERATED_BODY()
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference);

public:
	UPROPERTY(EditAnywhere)
	FVector Offset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
	float Radius = 0.f;

	UPROPERTY(EditAnywhere)
	float Coefficient = 1.f;

	UPROPERTY(EditAnywhere)
	EAttackType AttackType = EAttackType::Default;
};
