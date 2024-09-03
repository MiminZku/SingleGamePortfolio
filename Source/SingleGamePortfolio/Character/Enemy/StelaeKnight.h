// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Enemy/MonsterBase.h"
#include "StelaeKnight.generated.h"

/**
 * 
 */
UCLASS()
class SINGLEGAMEPORTFOLIO_API AStelaeKnight : public AMonsterBase
{
	GENERATED_BODY()
	
public:
	AStelaeKnight();

protected:
	virtual void PostInitializeComponents() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount,
		struct FDamageEvent const& DamageEvent, class AController* EventInstigator,
		AActor* DamageCauser) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

	virtual void AttackCollisionCheck(EAttackType AttackType) override;

	virtual void Attack() override;

	virtual void Die() override;

	FORCEINLINE FVector GetCollisionStartPos() { return mCollisionStartPos->GetComponentLocation(); }
	FORCEINLINE FVector GetCollisionEndPos() { return mCollisionEndPos->GetComponentLocation(); }
	FORCEINLINE FVector GetCollisionRadiusPos() { return mCollisionRadius->GetComponentLocation(); }
	FORCEINLINE FVector GetPrevCollisionPos() { return mPrevCollisionPos; }
	FORCEINLINE void SetPrevCollisionPos(const FVector& Vec) { mPrevCollisionPos = Vec; }

	void ResetAttackedCharacters();

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> mSword;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* mCollisionStartPos = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* mCollisionEndPos = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* mCollisionRadius = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector mPrevCollisionPos = FVector::ZeroVector;

	class IHitInterface* mHitInterface;
private:

};
