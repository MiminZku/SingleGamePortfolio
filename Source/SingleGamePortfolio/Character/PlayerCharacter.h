// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Animation/PlayerAnimTemplate.h"
#include "../Interface/AttackInterface.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UPlayerAnimTemplate;
struct FInputActionValue;
class APlayerWeapon;

UCLASS()
class SINGLEGAMEPORTFOLIO_API APlayerCharacter : public ACharacter, public IAttackInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

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
	void StopMove(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);
	void Run(const FInputActionValue& Value);
	void WeakAttack(const FInputActionValue& Value);
	void StrongAttack(const FInputActionValue& Value);
	void Dash(const FInputActionValue& Value);
	void ArmUnarm(const FInputActionValue& Value);
	virtual void Arm();
	virtual void Unarm();
	virtual void Attack(bool IsWeak);

public:
	virtual void GrabWeapon();
	virtual void HolsterWeapon();
	virtual void AttackCollisionCheck(EAttackType AttackType) override;
	virtual void AttackCollisionCheckOnce(FVector Offset, float Radius,
		EAttackType AttackType) override;
	virtual float TakeDamage(float DamageAmount,
		struct FDamageEvent const& DamageEvent, class AController* EventInstigator,
		AActor* DamageCauser) override;

	void PickWeaponUp(APlayerWeapon* Weapon);
	void ResetAttackedCharacters();
	void HitStop(float NewTimeDilation, float Duration);

	void SetState(EPlayerState State)
	{
		mState = State;
		if (IsValid(mAnimInstance))
		{
			mAnimInstance->SetState(mState);
		}
	}
	void SetDodgeEnable(bool b) { bCanDodge = b; bIsDodging = !b; }
	void SetAttackEnable(bool Enable) { bCanAttack = Enable; }
	void SetJumpEnable(bool Enable) { bCanJump = Enable; }
	void SetRunEnable(bool Enable) { bCanRun = Enable; }
	void SetCurrnetAttack(const FName& String) { mCurrentAttack = String; }
	void SetHasWeapon(bool b) { bHasWeapon = b; }
	void SetDamaged(bool Enable) { bDamaged = Enable; }

	bool HasWeapon() const { return bHasWeapon; }
	bool GetJumpEnable() { return bCanJump; }
	FVector GetMoveVector() { return mMoveVector; }
	APlayerWeapon* GetWeapon() { return mWeapon; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* mCameraArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* mCamera;

	TObjectPtr<UPlayerAnimTemplate> mAnimInstance;

	EPlayerState mState = EPlayerState::UnArmed;

	FVector mMoveVector = FVector::ZeroVector;

	float mWalkSpeed = 300.f;

	bool bIsDodging = false;

	bool bCanDodge = true;

	bool bCanAttack = true;

	bool bCanJump = true;

	bool bCanRun = true;

	bool bHasWeapon = false;

	bool bDamaged = false;

	UPROPERTY(EditAnywhere)
	bool bDrawDebug = false;

	FName mCurrentAttack = TEXT("Idle");

	UPROPERTY()
	APlayerWeapon* mWeapon = nullptr;

	TArray<IAttackInterface*> mAttackedCharacters;

private:
	struct FEnhancedInputActionValueBinding* mMoveActionBinding;
};
