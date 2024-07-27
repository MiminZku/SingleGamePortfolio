// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BasicCharacter.h"
#include "GreatSwordPlayer.generated.h"

UCLASS()
class SINGLEGAMEPORTFOLIO_API AGreatSwordPlayer : public ABasicCharacter
{
	GENERATED_BODY()
	
public:
	AGreatSwordPlayer();

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

protected:
	virtual void Arm() override;
	virtual void Unarm() override;
	virtual void Attack(bool IsWeak);

private:

};
