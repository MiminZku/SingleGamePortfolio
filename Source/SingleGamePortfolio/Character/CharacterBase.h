// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

UCLASS()
class SINGLEGAMEPORTFOLIO_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

protected:
	virtual void PostInitializeComponents() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount,
		struct FDamageEvent const& DamageEvent, class AController* EventInstigator,
		AActor* DamageCauser) override;

	virtual void SetCollisionEnable(bool Enable);

public:
	virtual void Die();

public:
	UPROPERTY(EditAnywhere)
	bool bDrawDebug = true;

protected:

// Stat Section
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCharacterStatComponent> mStats;

// UI WIdget Section
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWidgetComponent> mHpBar;

	TObjectPtr<class UAnimInstance> mAnimInstance;
};
