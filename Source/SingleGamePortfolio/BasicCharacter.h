// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Character/PlayerDefaultAnimTemplate.h"
#include "BasicCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UPlayerDefaultAnimTemplate;
struct FInputActionValue;

UCLASS()
class SINGLEGAMEPORTFOLIO_API ABasicCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABasicCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);
	void WeakAttack(const FInputActionValue& Value);
	void StrongAttack(const FInputActionValue& Value);
	void Dash(const FInputActionValue& Value);
	void StopDash(const FInputActionValue& Value);
	void ArmUnarm(const FInputActionValue& Value);
	void Arm();
	void Unarm();

	void SetState(EPlayerState State);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* mCameraArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* mCamera;

	TObjectPtr<UPlayerDefaultAnimTemplate> mAnimInstance;

	EPlayerState mState;

	float mWalkSpeed = 400.f;
};
