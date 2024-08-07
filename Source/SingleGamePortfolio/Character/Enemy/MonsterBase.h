// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AICharacter.h"
#include "Interface/AttackInterface.h"
#include "MonsterBase.generated.h"

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

protected:

private:

};
