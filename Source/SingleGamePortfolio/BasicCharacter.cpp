// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Input/DefaultInput.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Item/PlayerWeapon.h"
#include "Engine/DamageEvents.h"

// Sets default values
ABasicCharacter::ABasicCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	//GetCharacterMovement()->AirControl = 0.5f;
	GetCharacterMovement()->MaxWalkSpeed = mWalkSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	//GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	//GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	mCameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	mCameraArm->SetupAttachment(RootComponent);
	mCameraArm->TargetArmLength = 500.0f; // The camera follows at this distance behind the character	
	mCameraArm->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	mCameraArm->bEnableCameraLag = true;
	mCameraArm->CameraLagSpeed = 5.f;

	// Create a follow camera
	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	mCamera->SetupAttachment(mCameraArm); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	mCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));
}

// Called when the game starts or when spawned
void ABasicCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABasicCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Black,
	//	GetVelocity().ToString());

	// 멈추면 다시 걷기
	if (GetVelocity().IsNearlyZero(0.01))
	{
		GetCharacterMovement()->MaxWalkSpeed = mWalkSpeed;
	}
}

// Called to bind functionality to input
void ABasicCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// 클래스 디폴트 오브젝트(CDO)를 얻어온다.
			const UDefaultInput* Input = GetDefault<UDefaultInput>();

			Subsystem->AddMappingContext(Input->GetInputMappingContext(), 0);
		}
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		const UDefaultInput* InputData = GetDefault<UDefaultInput>();

		// Moving
		EnhancedInputComponent->BindAction(InputData->GetMoveInputAction(),
			ETriggerEvent::Triggered, this, &ABasicCharacter::Move);
		mMoveActionBinding = &EnhancedInputComponent->BindActionValue(InputData->GetMoveInputAction());

		// Looking
		EnhancedInputComponent->BindAction(InputData->GetLookInputAction(),
			ETriggerEvent::Triggered, this, &ABasicCharacter::Look);

		// Jumping
		EnhancedInputComponent->BindAction(InputData->GetJumpInputAction(),
			ETriggerEvent::Started, this, &ABasicCharacter::Jump);
		EnhancedInputComponent->BindAction(InputData->GetJumpInputAction(),
			ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Running
		EnhancedInputComponent->BindAction(InputData->GetRunInputAction(),
			ETriggerEvent::Started, this, &ABasicCharacter::Run);

		// Attacking
		EnhancedInputComponent->BindAction(InputData->GetWeakAttackInputAction(),
			ETriggerEvent::Started, this, &ABasicCharacter::WeakAttack);
		EnhancedInputComponent->BindAction(InputData->GetStrongAttackInpuAction(),
			ETriggerEvent::Started, this, &ABasicCharacter::StrongAttack);

		// Dash
		EnhancedInputComponent->BindAction(InputData->GetDashInputAction(),
			ETriggerEvent::Started, this, &ABasicCharacter::Dash);

		// Arm, Unarm
		EnhancedInputComponent->BindAction(InputData->GetArmUnarmAction(),
			ETriggerEvent::Started, this, &ABasicCharacter::ArmUnarm);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ABasicCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	mMoveVector = Value.Get<FVector>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, mMoveVector.X);
		AddMovementInput(RightDirection, mMoveVector.Y);
	}
}

void ABasicCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ABasicCharacter::Jump(const FInputActionValue& Value)
{
	if (!bCanJump) return;
	ACharacter::Jump();
}

void ABasicCharacter::Run(const FInputActionValue& Value)
{
	if (!bCanRun) return;
	if (GetCharacterMovement()->IsFalling())	return;
	//if (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Flying) return;
	if (EPlayerState::Armed == mState)
	{
		Unarm();
	}
	GetCharacterMovement()->MaxWalkSpeed = 2 * mWalkSpeed;
}

void ABasicCharacter::WeakAttack(const FInputActionValue& Value)
{
	Attack(true);
}

void ABasicCharacter::StrongAttack(const FInputActionValue& Value)
{
	Attack(false);
}

void ABasicCharacter::Dash(const FInputActionValue& Value)
{
	if (EPlayerState::Armed == mState)
	{
		if (!bCanDodge)	return;
		FVector Vec = mMoveActionBinding->GetValue().Get<FVector>();
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Black, Vec.ToString());
		if (Vec.IsNearlyZero(0.0001))		return;
		if (GetCharacterMovement()->IsFalling())	return;
		bCanDodge = false;
		bIsDodging = true;
		bCanJump = false;

		FVector DirWannaGo = GetActorForwardVector() * Vec.X + GetActorRightVector() * Vec.Y;
		float Direction = mAnimInstance->CalculateDirection(DirWannaGo, GetActorRotation());
		int32 Option = FMath::Floor(int(Direction + 45 + 90 + 360) % 360 / 90);
		switch (Option)
		{
		case 0:
			mAnimInstance->PlayMontage(TEXT("Dash"), TEXT("L"));
			break;
		case 1:
			mAnimInstance->PlayMontage(TEXT("Dash"), TEXT("F"));
			break;
		case 2:
			mAnimInstance->PlayMontage(TEXT("Dash"), TEXT("R"));
			break;
		case 3:
			mAnimInstance->PlayMontage(TEXT("Dash"), TEXT("B"));
			break;
		default:
			break;
		}
	}
	else if (EPlayerState::UnArmed == mState)
	{
	}
}

void ABasicCharacter::ArmUnarm(const FInputActionValue& Value)
{
	if (EPlayerState::Armed == mState)
	{
		Unarm();
	}
	else if (EPlayerState::UnArmed == mState)
	{
		Arm();
	}
}

void ABasicCharacter::Arm()
{
	SetState(EPlayerState::Armed);
	bUseControllerRotationYaw = true;
	mAnimInstance->PlayMontage(TEXT("ArmUnarm"), TEXT("Arm"));
	SetAttackEnable(false);
}

void ABasicCharacter::Unarm()
{
	SetState(EPlayerState::UnArmed);
	bUseControllerRotationYaw = false;
	mAnimInstance->PlayMontage(TEXT("ArmUnarm"), TEXT("Unarm"));
	mCurrentAttack = TEXT("Idle");
}

void ABasicCharacter::Attack(bool IsWeak)
{
	if (EPlayerState::UnArmed == mState)
	{
		Arm();
		return;
	}
	if (EPlayerState::Armed != mState)	return;
	if (!bCanAttack || bIsDodging)	return;
	if (GetCharacterMovement()->IsFalling())
	{
		mAnimInstance->PlayMontage(TEXT("Attack"), TEXT("Air"));
		bCanAttack = false;
		bCanJump = false;
		return;
	}
	if (GetCharacterMovement()->IsFalling())	return;

	bCanAttack = false;
	bCanJump = false;
	bCanDodge = false;
	bCanRun = false;

	FString NextAttack = mAnimInstance->GetNextAttackSection(mCurrentAttack, IsWeak);
	if (!NextAttack.Compare(TEXT(""))) return;

	mAnimInstance->PlayMontage(TEXT("Attack"), *NextAttack);
	mCurrentAttack = NextAttack;
}

void ABasicCharacter::AttackCollisionCheck(EAttackType AttackType)
{
}

void ABasicCharacter::AttackCollisionCheckOnce(FVector Offset,
	float Radius, EAttackType AttackType)
{
	FVector Origin = GetActorLocation() + Offset;
	FCollisionQueryParams Params(NAME_None, false, this);
	TArray<FHitResult> HitResults;
	bool Collision = GetWorld()->SweepMultiByChannel(OUT HitResults,
		Origin, Origin, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel3,
		FCollisionShape::MakeSphere(Radius), Params);

	if (Collision)
	{
		for (const FHitResult& HitResult : HitResults)
		{
			INormalAttackInterface* AttackedCharacter = Cast<INormalAttackInterface>(HitResult.GetActor());
			if (AttackedCharacter)
			{
				if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green,
					FString::Printf(TEXT("%s"), *HitResult.GetActor()->GetName()));

				HitStop(0.1f, 0.01f);

				FDamageEvent Dmg;
				AttackedCharacter->Attacked(1.f, Dmg, GetController(), this,
					AttackType);
			}
		}
	}

#if ENABLE_DRAW_DEBUG
	if (bDrawDebug)
	{
		FColor DrawColor = Collision ? FColor::Green : FColor::Red;

		DrawDebugSphere(GetWorld(), Origin, Radius, 26, DrawColor, false, 1.f);
		//DrawDebugCapsule(GetWorld(), Origin, CapsuleHalfHeight,
		//	Radius, FRotationMatrix::MakeFromZ((End - Start)).ToQuat(),
		//	DrawColor, false, 1);
	}
#endif

}

void ABasicCharacter::Attacked(float DamageAmount,
	struct FDamageEvent const& DamageEvent, class AController* EventInstigator,
	AActor* DamageCauser, EAttackType AttackType)
{
	switch (AttackType)
	{
	case EAttackType::Default:
		GetCharacterMovement()->AddImpulse(FVector::UpVector * 1000.f);
		break;
	case EAttackType::Airborn:

		break;
	case EAttackType::Knockback:
		break;
	case EAttackType::KnockDown:
		break;
	case EAttackType::Slow:
		break;
	case EAttackType::Stun:
		break;
	default:
		break;
	}
}

void ABasicCharacter::GrabWeapon()
{
}

void ABasicCharacter::HolsterWeapon()
{
}

void ABasicCharacter::PickWeaponUp(APlayerWeapon* Weapon)
{
	SetHasWeapon(true);
	mWeapon = Weapon;
	mWeapon->AttachToComponent(GetMesh(),
		FAttachmentTransformRules::SnapToTargetIncludingScale,
		TEXT("unequiped_weapon"));
}

void ABasicCharacter::ResetAttackedCharacters()
{
	for (INormalAttackInterface* AttackedCharacter : mAttackedCharacters)
	{
		AttackedCharacter->SetDamaged(false);
	}
	mAttackedCharacters.Empty();
}

void ABasicCharacter::HitStop(float NewTimeDilation, float Duration)
{
	// 새로운 시간 왜곡 값 설정
	GetWorld()->GetWorldSettings()->SetTimeDilation(NewTimeDilation);

	// 일정 시간이 지난 후 원래 값으로 되돌리기
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
		{
			// 원래 시간 왜곡 값으로 되돌리기
			GetWorld()->GetWorldSettings()->SetTimeDilation(1.f);
		}, Duration, false); // false는 반복하지 않도록 설정
}


