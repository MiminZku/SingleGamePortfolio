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
class APlayerWeapon;

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
	virtual void Arm();
	virtual void Unarm();
	virtual void Attack(bool IsWeak);

public:
	virtual void GrabWeapon();
	virtual void HolsterWeapon();

	void PickWeaponUp(APlayerWeapon* Weapon);

	void SetState(EPlayerState State);
	void SetDodgeEnable(bool b) { bCanDodge = b; bIsDodging = !b; }
	void SetAttackEnable(bool Enable) { bCanAttack = Enable; }
	void SetJumpEnable(bool Enable) { bCanJump = Enable; }
	void SetCurrnetAttack(const FString& String) { mCurrentAttack = String; }
	void SetHasWeapon(bool b) { bHasWeapon = b; }

	FVector GetMoveVector() const { return mMoveVector; }
	bool HasWeapon() const { return bHasWeapon; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* mCameraArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* mCamera;

	TObjectPtr<UPlayerDefaultAnimTemplate> mAnimInstance;

	EPlayerState mState = EPlayerState::UnArmed;

	FVector mMoveVector = FVector::ZeroVector;

	float mWalkSpeed = 300.f;

	bool bIsDodging = false;

	bool bCanDodge = true;

	bool bCanAttack = true;

	bool bCanJump = true;

	bool bHasWeapon = false;

	FString mCurrentAttack = TEXT("Idle");

	UPROPERTY()
	APlayerWeapon* mWeapon = nullptr;

private:
	struct FEnhancedInputActionValueBinding* mMoveActionBinding;
};
