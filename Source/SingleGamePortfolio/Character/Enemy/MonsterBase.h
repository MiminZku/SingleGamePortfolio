// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AICharacter.h"
#include "Interface/AttackInterface.h"
#include "Animation/MonsterAnimTemplate.h"
#include "MonsterBase.generated.h"

DECLARE_DELEGATE(FMonsterAttackFinished);
/**
 * 
 */
UCLASS()
class SINGLEGAMEPORTFOLIO_API AMonsterBase : public AAICharacter, public IAttackInterface
{
	GENERATED_BODY()
	
public:
	AMonsterBase();

	virtual void AttackCollisionCheck(EAttackType AttackType) override;
	virtual void AttackCollisionCheckOnce(FVector Offset, float Radius,
		EAttackType AttackType) override;

	virtual void Attack();

	virtual void SetState(EMonsterState InState);

protected:


private:


public:
	FMonsterAttackFinished OnAttackFinished;

protected:
	EMonsterState mState;
};
