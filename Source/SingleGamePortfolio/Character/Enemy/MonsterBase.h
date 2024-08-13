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

	virtual void AttackCollisionCheck() override;
	virtual void AttackCollisionCheckOnce(FVector Offset, float Radius) override;

	virtual void Activate();
	virtual void Deactivate();

	virtual void Angry();
	virtual void Attack();
	virtual void SetState(EMonsterState InState);

	virtual void Die() override;

	void BindSpawner(class AMonsterSpawner* Spawner);
	void DetectedTarget(APawn* Target);
	void RegisterTarget(APawn* Target);
	
	bool CanAttack() { return bCanAttack; }
	void SetAttackEnable(bool Enable) { bCanAttack = Enable; }

	bool IsActive() { return bIsActive; }
	void SetActive(bool Active) { bIsActive = Active; }

	bool HasTarget() { return nullptr != mTarget; }
	void SetTarget(TObjectPtr<APawn> Target) { mTarget = Target; }

protected:


public:
	FMonsterAttackFinished OnAttackFinished;
	FMonsterAngryFinished OnAngryFinished;

protected:
	EMonsterState mState;

	UPROPERTY()
	TObjectPtr<class AMonsterSpawner> mSpawner;

	UPROPERTY()
	TObjectPtr<APawn> mTarget;

private:
	bool bCanAttack = true;

	bool bIsActive = false;
};
