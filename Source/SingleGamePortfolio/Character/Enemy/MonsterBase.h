// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AICharacter.h"
#include "Interface/AttackInterface.h"
#include "Animation/MonsterAnimTemplate.h"
#include "MonsterBase.generated.h"

DECLARE_DELEGATE(FMonsterAttackFinished);
DECLARE_DELEGATE(FMonsterAngryFinished);
/**
 * 
 */
UCLASS()
class SINGLEGAMEPORTFOLIO_API AMonsterBase : public AAICharacter, public IAttackInterface
{
	GENERATED_BODY()
	
public:
	AMonsterBase();
	
	virtual float TakeDamage(float DamageAmount,
		struct FDamageEvent const& DamageEvent, class AController* EventInstigator,
		AActor* DamageCauser) override;

	virtual void AttackCollisionCheck(EAttackType AttackType) override;
	virtual void AttackCollisionCheckOnce(FVector Offset, float Radius,
		EAttackType AttackType) override;

	virtual void Angry();
	virtual void Attack();
	virtual void SetState(EMonsterState InState);

	virtual void Die() override;

	bool CanAttack() { return bCanAttack; }
	void SetAttackEnable(bool Enable) { bCanAttack = Enable; }

protected:


public:
	FMonsterAttackFinished OnAttackFinished;
	FMonsterAngryFinished OnAngryFinished;

protected:
	EMonsterState mState;

private:
	bool bCanAttack = true;
};
