// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MonsterAnimTemplate.generated.h"

UENUM(BlueprintType)
enum class EMonsterState : uint8
{
	Idle,
	Walk,
	Run,
	Dead,
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

protected:


private:


protected:

};
