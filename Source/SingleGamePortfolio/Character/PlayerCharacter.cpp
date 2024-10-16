// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
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
#include "CharacterStat/CharacterStatComponent.h"
#include "Interface/AttackInterface.h"
#include "Interface/HitInterface.h"
#include "UI/ProgressBarWidget.h"
#include "Components/WidgetComponent.h"
#include "Item/ItemBox.h"
#include "MotionWarpingComponent.h"
#include "UI/HUDWidget.h"
//#include "Kismet/GameplayStatics.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

	SetTarget(nullptr);

	mTargetWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Target"));
	mTargetWidget->SetupAttachment(RootComponent);
	const static ConstructorHelpers::FClassFinder<UUserWidget>
		TargetUI(TEXT("/Game/_Programming/UI/WBP_Target.WBP_Target_C"));
	if (TargetUI.Class)
	{
		mTargetWidget->SetWidgetClass(TargetUI.Class);
		mTargetWidget->SetDrawSize(FVector2D(50.f, 100.f));
		mTargetWidget->SetWidgetSpace(EWidgetSpace::Screen);
		mTargetWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	//mMotionWarping = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarping"));

}

void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	mTargetWidget->InitWidget();
	mTargetWidget->SetHiddenInGame(true);

	//mStats->SetStats(1);
	mStats->OnLevelUp.AddUObject(this, &APlayerCharacter::LevelUp);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	bDrawDebug = false;

	// 커서 이미지 변경 테스트 코드
	//auto* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//if (PlayerController)
	//{
	//	PlayerController->SetMouseCursorWidget(EMouseCursor::Default, mTargetWidget->GetUserWidgetObject());

	//	// Doesn't work
	//	//PlayerController->SetMouseCursorWidget(EMouseCursor::GrabHand, mTargetWidget->GetUserWidgetObject());
	//	//PlayerController->CurrentMouseCursor = EMouseCursor::GrabHand;
	//	//PlayerController->DefaultMouseCursor = EMouseCursor::GrabHand;
	//}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	mStats->RecoverMp(DeltaTime * 5.f);

	// 멈추면 다시 걷기
	if (GetVelocity().IsNearlyZero(0.01) || mStats->GetCurrentStat(TEXT("Mp")) < 0.1f)
	{
		GetCharacterMovement()->MaxWalkSpeed = mWalkSpeed;
	}
	if (GetCharacterMovement()->MaxWalkSpeed > mWalkSpeed)
	{
		mStats->UseMp(DeltaTime * 10.f);
	}

	if (IsValid(mTarget))
	{
		if (Controller)
			Controller->SetControlRotation(
				FMath::RInterpTo(GetControlRotation(),
					(mTarget->GetActorLocation() - 
						(GetActorLocation() + 
							FVector::UpVector * (0.5f * FVector::Dist(mTarget->GetActorLocation(), GetActorLocation())))
						).Rotation(),
					DeltaTime, 10.f));
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
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
			ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		//mMoveActionBinding = &EnhancedInputComponent->BindActionValue(InputData->GetMoveInputAction());
		EnhancedInputComponent->BindAction(InputData->GetMoveInputAction(),
			ETriggerEvent::Completed, this, &APlayerCharacter::StopMove);

		// Looking
		EnhancedInputComponent->BindAction(InputData->GetLookInputAction(),
			ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		EnhancedInputComponent->BindAction(InputData->GetLookInputAction(),
			ETriggerEvent::Completed, this, &APlayerCharacter::StopLook);

		// Jumping
		EnhancedInputComponent->BindAction(InputData->GetJumpInputAction(),
			ETriggerEvent::Started, this, &APlayerCharacter::Jump);
		EnhancedInputComponent->BindAction(InputData->GetJumpInputAction(),
			ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Running
		EnhancedInputComponent->BindAction(InputData->GetRunInputAction(),
			ETriggerEvent::Started, this, &APlayerCharacter::Run);

		// Attacking
		EnhancedInputComponent->BindAction(InputData->GetWeakAttackInputAction(),
			ETriggerEvent::Started, this, &APlayerCharacter::WeakAttack);
		EnhancedInputComponent->BindAction(InputData->GetStrongAttackInpuAction(),
			ETriggerEvent::Started, this, &APlayerCharacter::StrongAttack);

		// Dash
		EnhancedInputComponent->BindAction(InputData->GetDashInputAction(),
			ETriggerEvent::Started, this, &APlayerCharacter::Dash);

		// Arm, Unarm
		EnhancedInputComponent->BindAction(InputData->GetArmUnarmAction(),
			ETriggerEvent::Started, this, &APlayerCharacter::ArmUnarm);

		// Camera Lock On Target
		EnhancedInputComponent->BindAction(InputData->GetLockOnAction(),
			ETriggerEvent::Started, this, &APlayerCharacter::LockOnAction);
		EnhancedInputComponent->BindAction(InputData->GetLockOnAction(),
			ETriggerEvent::Completed, this, &APlayerCharacter::LockOffAction);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	mMoveInputVec = Value.Get<FVector>();

	if (Controller)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, mMoveInputVec.X);
		AddMovementInput(RightDirection, mMoveInputVec.Y);
	}
}
void APlayerCharacter::StopMove(const FInputActionValue& Value)
{
	mMoveInputVec = FVector::ZeroVector;
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	if (IsValid(mTarget)) return;	// when lock on target

	// input is a Vector2D
	mLookInputVec = Value.Get<FVector>();

	if (Controller)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(mLookInputVec.X);
		AddControllerPitchInput(mLookInputVec.Y);
	}
}
void APlayerCharacter::StopLook(const FInputActionValue& Value)
{
	mLookInputVec = FVector::ZeroVector;
}

void APlayerCharacter::Jump(const FInputActionValue& Value)
{
	if (!bCanJump) return;
	ACharacter::Jump();
}

void APlayerCharacter::Run(const FInputActionValue& Value)
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

void APlayerCharacter::WeakAttack(const FInputActionValue& Value)
{
	Attack(true);
}

void APlayerCharacter::StrongAttack(const FInputActionValue& Value)
{
	Attack(false);
}

void APlayerCharacter::Dash(const FInputActionValue& Value)
{
	UPlayerAnimTemplate* AnimInstance = Cast<UPlayerAnimTemplate>(mAnimInstance);
	if (nullptr == AnimInstance)	return;

	if (!bCanDodge)	return;
	if (mMoveInputVec.IsNearlyZero(0.0001))		return;
	if (GetCharacterMovement()->IsFalling())	return;
	if (mStats->GetCurrentStat(TEXT("Mp")) < 20.f)
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red,
			TEXT("Not enough Mp!"));
		return;
	}

	mStats->UseMp(20.f);
	bCanDodge = false;
	bIsDodging = true;
	bCanJump = false;
	SetCollisionEnable(false);


	if (EPlayerState::Armed == mState)
	{
		//FVector Vec = mMoveActionBinding->GetValue().Get<FVector>();
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Black, Vec.ToString());

		FVector DirWannaGo =
			GetActorForwardVector() * mMoveInputVec.X + GetActorRightVector() * mMoveInputVec.Y;
		float Direction = mAnimInstance->CalculateDirection(DirWannaGo, GetActorRotation());
		int32 Option = FMath::Floor(int(Direction + 45 + 90 + 360) % 360 / 90);
		switch (Option)
		{
		case 0:
			AnimInstance->PlayMontage(TEXT("Dash"), TEXT("L"));
			break;
		case 1:
			AnimInstance->PlayMontage(TEXT("Dash"), TEXT("F"));
			break;
		case 2:
			AnimInstance->PlayMontage(TEXT("Dash"), TEXT("R"));
			break;
		case 3:
			AnimInstance->PlayMontage(TEXT("Dash"), TEXT("B"));
			break;
		default:
			break;
		}
	}
	else if (EPlayerState::UnArmed == mState)
	{
		AnimInstance->PlayMontage(TEXT("Dash"), TEXT("Unarm"));
	}
}

void APlayerCharacter::ArmUnarm(const FInputActionValue& Value)
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

void APlayerCharacter::Arm()
{
	if (bIsDodging) return;
	SetState(EPlayerState::Armed);
	bUseControllerRotationYaw = true;
	Cast<UPlayerAnimTemplate>(mAnimInstance)->PlayMontage(TEXT("ArmUnarm"), TEXT("Arm"));
	// 애니메이션에서 검 잡기 전까지
	SetAttackEnable(false);
	SetDodgeEnable(false);
	SetRunEnable(false);
}

void APlayerCharacter::Unarm()
{
	if (bIsDodging) return;
	SetState(EPlayerState::UnArmed);
	bUseControllerRotationYaw = false;
	Cast<UPlayerAnimTemplate>(mAnimInstance)->PlayMontage(TEXT("ArmUnarm"), TEXT("Unarm"));
	mCurrentAttack = TEXT("Idle");
	SetDodgeEnable(false);
	SetRunEnable(false);
}

void APlayerCharacter::Attack(bool IsWeak)
{
	UPlayerAnimTemplate* AnimInstance = Cast<UPlayerAnimTemplate>(mAnimInstance);
	if (nullptr == AnimInstance) return;

	if (EPlayerState::UnArmed == mState)
	{
		Arm();
		return;
	}
	if (EPlayerState::Armed != mState)	return;
	if (!bCanAttack || bIsDodging)	return;
	if (GetCharacterMovement()->IsFalling())
	{
		AnimInstance->PlayMontage(TEXT("Attack"), IsWeak ? TEXT("AirWeak") : TEXT("AirStrong"));
		bCanAttack = false;
		bCanJump = false;
		return;
	}
	if (GetCharacterMovement()->IsFalling())	return;

	bCanAttack = false;
	bCanJump = false;
	bCanDodge = false;
	bCanRun = false;

	FName NextAttack = AnimInstance->GetNextAttackSection(mCurrentAttack, IsWeak);
	if (!NextAttack.Compare(TEXT(""))) return;

	AnimInstance->PlayMontage(TEXT("Attack"), NextAttack);
	mCurrentAttack = NextAttack;
}

void APlayerCharacter::LockOnAction(const FInputActionValue& Value)
{
	FVector Origin = GetActorLocation();
	FCollisionQueryParams Params(NAME_None, false, this);
	TArray<FHitResult> HitResults;
	float Radius = 1500.f;
	bool bCollision = GetWorld()->SweepMultiByChannel(OUT HitResults,
		Origin, Origin, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel3,
		FCollisionShape::MakeSphere(Radius), Params);
	float TargetScore = std::numeric_limits<float>::min();
	ACharacter* Result = nullptr;
	if (bCollision)
	{
		for (const FHitResult& HitResult : HitResults)
		{
			ACharacter* Target = Cast<ACharacter>(HitResult.GetActor());
			if (Target)
			{
				FVector VecToTarget = Target->GetActorLocation() - GetActorLocation();
				float DistToTarget = VecToTarget.Length();
				VecToTarget.Normalize();
				float DotValue = FVector::DotProduct(GetBaseAimRotation().Vector(), VecToTarget);
				float Score = DotValue / DistToTarget;
				if (Score > TargetScore)
				{
					TargetScore = Score;
					Result = Target;
				}
			}
		}
		//if (IsValid(mTarget))
		//	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, Result->GetName());
		SetTarget(Result);
		if (Result)
		{
			mTargetWidget->SetHiddenInGame(false);
			FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, false);
			mTargetWidget->AttachToComponent(Result->GetCapsuleComponent(), Rules);
			mTargetWidget->SetRelativeLocation(FVector(0.f, 0.f, 50.f + Result->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
		}
	}

#if ENABLE_DRAW_DEBUG
	FColor DrawColor = bCollision ? FColor::Green : FColor::Red;
	DrawDebugSphere(GetWorld(), Origin, Radius, 26, DrawColor, false, 1.f);
#endif
}

void APlayerCharacter::LockOffAction(const FInputActionValue& Value)
{
	if (IsValid(mTarget))
	{
		LockOff();
	}
}

void APlayerCharacter::AttackCollisionCheck(EAttackType AttackType)
{
	//if (!IsValid(mTarget))	return;
	//FTransform TargetTransform((mTarget->GetActorLocation() - GetActorLocation()).Rotation(), mTarget->GetActorLocation(), FVector(1.f));
	//FMotionWarpingTarget MotionWarpingTarget(TEXT("Attack"), TargetTransform);
	//mMotionWarping->AddOrUpdateWarpTarget(MotionWarpingTarget);
}

void APlayerCharacter::AttackCollisionCheckOnce(EAttackType AttackType, FVector Offset, float Radius, float Coefficient)
{
	mStats->UseMp(30.f);

	FVector Origin = GetActorLocation() + Offset;
	FCollisionQueryParams Params(NAME_None, false, this);
	TArray<FHitResult> HitResults;
	bool bCollision = GetWorld()->SweepMultiByChannel(OUT HitResults,
		Origin, Origin, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel3,
		FCollisionShape::MakeSphere(Radius), Params);

	if (bCollision)
	{
		for (const FHitResult& HitResult : HitResults)
		{
			IHitInterface* AttackedActor = Cast<IHitInterface>(HitResult.GetActor());
			if (AttackedActor)
			{
				AttackedActor->Execute_GetHit(HitResult.GetActor(), HitResult.ImpactPoint);
				AttackedActor->Execute_HitEvent(HitResult.GetActor(), HitResult.ImpactPoint, AttackType);

				HitStop(0.1f, 0.01f);

				FDamageEvent DmgEvent;
				HitResult.GetActor()->TakeDamage(mStats->GetAtk() * Coefficient, DmgEvent, GetController(), this);

				AItemBox* ItemBox = Cast<AItemBox>(HitResult.GetActor());
				if (ItemBox)
				{
					mWeapon->CreateFields(HitResult.ImpactPoint);
					FTimerHandle TimerHandle;
					GetWorld()->GetTimerManager().SetTimer(TimerHandle,
						[this, HitResult]()
						{
							mWeapon->CreateFields(HitResult.ImpactPoint);
						},
						0.01f, false);
				}
			}
		}
	}

#if ENABLE_DRAW_DEBUG
	if (bDrawDebug)
	{
		FColor DrawColor = bCollision ? FColor::Green : FColor::Red;

		DrawDebugSphere(GetWorld(), Origin, Radius, 26, DrawColor, false, 1.f);
		//DrawDebugCapsule(GetWorld(), Origin, CapsuleHalfHeight,
		//	Radius, FRotationMatrix::MakeFromZ((End - Start)).ToQuat(),
		//	DrawColor, false, 1);
	}
#endif

}

void APlayerCharacter::GetHit_Implementation(const FVector& ImpactPoint)
{
	SetDamaged(true);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	bCanJump = false;
	bCanAttack = false;

	FVector ForwardVec = GetActorForwardVector();
	FVector VecToImpactPoint = ImpactPoint - GetActorLocation();

	// 앞 뒤 판단
	float DotRes = FVector::DotProduct(ForwardVec, VecToImpactPoint);
	if (DotRes < 0)	// 뒤에서 공격 받았을 때
	{
		Cast<UPlayerAnimTemplate>(mAnimInstance)->PlayMontage(TEXT("Hit"), TEXT("Back"));
		return;
	}

	// 좌 우 판단
	FVector CrossRes = FVector::CrossProduct(ForwardVec, VecToImpactPoint);

	Cast<UPlayerAnimTemplate>(mAnimInstance)->PlayMontage(TEXT("Hit"),
		(CrossRes.Z > 0) ? TEXT("Right") : TEXT("Left"));

}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	return Damage;
}

void APlayerCharacter::Die()
{
	Super::Die();

	mAnimInstance->StopAllMontages(0.f);
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
	YouDie();
}

void APlayerCharacter::SetCollisionEnable(bool Enable)
{
	Super::SetCollisionEnable(Enable);

	if (Enable)
	{
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));
	}
	else
	{
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("IgnoreOnlyPawn"));
	}
}

void APlayerCharacter::GrabWeapon()
{
	SetAttackEnable(true);
	SetDodgeEnable(true);
	SetRunEnable(true);
}

void APlayerCharacter::HolsterWeapon()
{
	SetDodgeEnable(true);
	SetRunEnable(true);
}

void APlayerCharacter::PickWeaponUp(APlayerWeapon* Weapon)
{
	SetHasWeapon(true);
	mWeapon = Weapon;
	mWeapon->AttachToComponent(GetMesh(),
		FAttachmentTransformRules::SnapToTargetIncludingScale,
		TEXT("unequiped_weapon"));
}

void APlayerCharacter::ResetAttackedCharacters()
{
	for (IHitInterface* AttackedCharacter : mHitInterfaces)
	{
		AttackedCharacter->SetDamaged(false);
	}
	mHitInterfaces.Empty();
}

void APlayerCharacter::HitStop(float NewTimeDilation, float Duration)
{
	// 새로운 시간 왜곡 값 설정
	GetWorld()->GetWorldSettings()->SetTimeDilation(NewTimeDilation);

	// 일정 시간이 지난 후 원래 값으로 되돌리기
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
		{
			// 원래 시간 왜곡 값으로 되돌리기
			GetWorld()->GetWorldSettings()->SetTimeDilation(1.f);
		}, NewTimeDilation * Duration, false); // false는 반복하지 않도록 설정
}

void APlayerCharacter::LockOff()
{
	SetTarget(nullptr);
	FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, false);
	mTargetWidget->AttachToComponent(RootComponent, Rules);
	mTargetWidget->SetHiddenInGame(true);
}

void APlayerCharacter::SetupHUDWidget(UHUDWidget* InHUDWidget)
{
	if (InHUDWidget)
	{
		InHUDWidget->BindStats(mStats);
	}
}

void APlayerCharacter::LevelUp()
{

}
