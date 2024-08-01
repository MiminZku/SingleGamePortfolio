// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "../Interface/AttackInterface.h"
#include "ANS_AttackCollisionCheck.generated.h"


class APlayerCharacter;

UCLASS()
class SINGLEGAMEPORTFOLIO_API UANS_AttackCollisionCheck : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation, float TotalDuration,
		const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation, float FrameDeltaTime, 
		const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation, 
		const FAnimNotifyEventReference& EventReference) override;
	
public:
	UPROPERTY()
	APlayerCharacter* mOwningCharacter = nullptr;

	UPROPERTY(EditAnywhere)
	EAttackType AttackType = EAttackType::Default;
};
