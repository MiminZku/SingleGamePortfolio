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
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

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
	mCameraArm->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	mCameraArm->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	mCamera->SetupAttachment(mCameraArm); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	mCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

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

		// Looking
		EnhancedInputComponent->BindAction(InputData->GetLookInputAction(),
			ETriggerEvent::Triggered, this, &ABasicCharacter::Look);
		
		// Jumping
		EnhancedInputComponent->BindAction(InputData->GetJumpInputAction(),
			ETriggerEvent::Started, this, &ABasicCharacter::Jump);
		EnhancedInputComponent->BindAction(InputData->GetJumpInputAction(),
			ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Attacking
		EnhancedInputComponent->BindAction(InputData->GetWeakAttackInputAction(),
			ETriggerEvent::Started, this, &ABasicCharacter::WeakAttack);
		EnhancedInputComponent->BindAction(InputData->GetStrongAttackInpuAction(),
			ETriggerEvent::Started, this, &ABasicCharacter::StrongAttack);

		// Dash
		EnhancedInputComponent->BindAction(InputData->GetDashInputAction(),
			ETriggerEvent::Started, this, &ABasicCharacter::Dash);
		EnhancedInputComponent->BindAction(InputData->GetDashInputAction(),
			ETriggerEvent::Completed, this, &ABasicCharacter::StopDash);

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
	FVector2D MovementVector = Value.Get<FVector2D>();

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
		AddMovementInput(ForwardDirection, MovementVector.X);
		AddMovementInput(RightDirection, MovementVector.Y);
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
	ACharacter::Jump();
}

void ABasicCharacter::WeakAttack(const FInputActionValue& Value)
{
}

void ABasicCharacter::StrongAttack(const FInputActionValue& Value)
{
}

void ABasicCharacter::Dash(const FInputActionValue& Value)
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue,
		TEXT("Dash"));
	if (EPlayerState::Armed == mState)
	{

	}
	else if (EPlayerState::UnArmed == mState)
	{
		GetCharacterMovement()->MaxWalkSpeed = 2 * mWalkSpeed;

	}
}

void ABasicCharacter::StopDash(const FInputActionValue& Value)
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue,
		TEXT("StopDash"));
	GetCharacterMovement()->MaxWalkSpeed = mWalkSpeed;

}

void ABasicCharacter::ArmUnarm(const FInputActionValue& Value)
{
	if (EPlayerState::Armed == mState)
	{
		Arm();
	}
	else if (EPlayerState::UnArmed == mState)
	{
		Unarm();
	}
}

void ABasicCharacter::Arm()
{
	SetState(EPlayerState::UnArmed);
	bUseControllerRotationYaw = false;
}

void ABasicCharacter::Unarm()
{
	SetState(EPlayerState::Armed);
	bUseControllerRotationYaw = true;
}

void ABasicCharacter::SetState(EPlayerState State)
{
	mState = State;
	if (IsValid(mAnimInstance))
	{
		mAnimInstance->SetState(mState);
	}

}

