// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "Animation/PlayerAnimTemplate.h"
#include "Interface/AttackInterface.h"
#include "Interface/HitInterface.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UPlayerAnimTemplate;
struct FInputActionValue;
class APlayerWeapon;

UCLASS()
class SINGLEGAMEPORTFOLIO_API APlayerCharacter : public ACharacterBase, public IAttackInterface, public IHitInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	virtual void PostInitializeComponents() override;

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
	void StopLook(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);
	void Run(const FInputActionValue& Value);
	void WeakAttack(const FInputActionValue& Value);
	void StrongAttack(const FInputActionValue& Value);
	void Dash(const FInputActionValue& Value);
	void ArmUnarm(const FInputActionValue& Value);
	virtual void Arm();
	virtual void Unarm();
	virtual void Attack(bool IsWeak);
	void LockOnAction(const FInputActionValue& Value);
	void LockOffAction(const FInputActionValue& Value);

public:
	virtual void GrabWeapon();
	virtual void HolsterWeapon();
	virtual void AttackCollisionCheck(EAttackType AttackType) override;
	virtual void AttackCollisionCheckOnce(EAttackType AttackType, FVector Offset, float Radius, float Coefficient) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	virtual float TakeDamage(float DamageAmount,
		struct FDamageEvent const& DamageEvent, class AController* EventInstigator,
		AActor* DamageCauser) override;
	virtual void Die() override;
	virtual void SetCollisionEnable(bool Enable) override;

	UFUNCTION(BlueprintImplementableEvent)
	void YouDie();

	void PickWeaponUp(APlayerWeapon* Weapon);
	void ResetAttackedCharacters();
	void HitStop(float NewTimeDilation, float Duration);
	void LockOff();

	bool HasWeapon() const { return bHasWeapon; }
	bool GetJumpEnable() { return bCanJump; }
	FVector GetMoveVector() { return mMoveInputVec; }
	FVector GetLookInputVector() { return mLookInputVec; }
	APlayerWeapon* GetWeapon() { return mWeapon; }
	TObjectPtr<ACharacter> GetTarget() { return mTarget; }

	void SetState(EPlayerState State)
	{
		mState = State;
		if (IsValid(mAnimInstance))
		{
			UPlayerAnimTemplate* AnimInstance = Cast<UPlayerAnimTemplate>(mAnimInstance);
			if(AnimInstance) AnimInstance->SetState(mState);
		}
	}
	void SetDodgeEnable(bool b) { bCanDodge = b; bIsDodging = !b; }
	void SetAttackEnable(bool Enable) { bCanAttack = Enable; }
	void SetJumpEnable(bool Enable) { bCanJump = Enable; }
	void SetRunEnable(bool Enable) { bCanRun = Enable; }
	void SetCurrnetAttack(const FName& String) { mCurrentAttack = String; }
	void SetHasWeapon(bool b) { bHasWeapon = b; }
	void SetTarget(TObjectPtr<ACharacter> InTarget) { mTarget = InTarget; }

	void SetupHUDWidget(class UHUDWidget* InHUDWidget);
	void LevelUp();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* mCameraArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* mCamera;

	EPlayerState mState = EPlayerState::UnArmed;

	FVector mMoveInputVec = FVector::ZeroVector;

	FVector mLookInputVec = FVector::ZeroVector;

	float mWalkSpeed = 300.f;

	bool bIsDodging = false;

	bool bCanDodge = true;

	bool bCanAttack = true;

	bool bCanJump = true;

	bool bCanRun = true;

	bool bHasWeapon = false;

	FName mCurrentAttack = TEXT("Idle");

	UPROPERTY()
	APlayerWeapon* mWeapon = nullptr;

	UPROPERTY()
	TArray<class IHitInterface*> mHitInterfaces;

	UPROPERTY()
	TObjectPtr<ACharacter> mTarget;

	UPROPERTY()
	TObjectPtr<class UWidgetComponent> mTargetWidget;

	//UPROPERTY()
	//TObjectPtr<class UMotionWarpingComponent> mMotionWarping;

private:
	struct FEnhancedInputActionValueBinding* mMoveActionBinding;
};
