// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AICharacter.h"
#include "Interface/AttackInterface.h"
#include "Interface/HitInterface.h"
#include "Animation/MonsterAnimTemplate.h"
#include "MonsterBase.generated.h"

DECLARE_DELEGATE(FMonsterAttackFinished);
DECLARE_DELEGATE(FMonsterAngryFinished);
/**
 * 
 */
UCLASS()
class SINGLEGAMEPORTFOLIO_API AMonsterBase : public AAICharacter, public IAttackInterface, public IHitInterface
{
	GENERATED_BODY()
	
public:
	AMonsterBase();

protected:
	virtual void PostInitializeComponents() override;

public:	
	virtual float TakeDamage(float DamageAmount,
		struct FDamageEvent const& DamageEvent, class AController* EventInstigator,
		AActor* DamageCauser) override;

	virtual void AttackCollisionCheck(EAttackType AttackType) override;
	virtual void AttackCollisionCheckOnce(EAttackType AttackType, FVector Offset, float Radius, float Coefficient) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	virtual void HitEvent_Implementation(const FVector& ImpactPoint, EAttackType AttackType) override;

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
	APawn* GetTarget() { return mTarget; }
	void SetTarget(APawn* Target) { mTarget = Target; }

	float GetAttackRange() { return mAttackRange; }
	void SetAttackRange(float InValue) { mAttackRange = InValue; }

	float GetAttackRadius() { return mAttackRadius; }
	void SetAttackRadius(float InValue) { mAttackRadius = InValue; }

	bool IsBoss() { return bIsBoss; }

public:
	FMonsterAttackFinished OnAttackFinished;
	FMonsterAngryFinished OnAngryFinished;

protected:
	EMonsterState mState;

	UPROPERTY()
	TObjectPtr<class AMonsterSpawner> mSpawner;

	UPROPERTY()
	TObjectPtr<APawn> mTarget;

	// UI WIdget Section
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWidgetComponent> mHpBarWidget;

	UPROPERTY()
	TObjectPtr<class AActor> mExp;

	bool bIsBoss = false;

private:
	bool bCanAttack = true;

	bool bIsActive = false;

	float mAttackRange = 50.f;

	float mAttackRadius = 50.f;

};
