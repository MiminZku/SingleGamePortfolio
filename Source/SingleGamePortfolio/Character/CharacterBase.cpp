// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterBase.h"
#include "CharacterStat/CharacterStatComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/HpBarWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->bReceivesDecals = false;
	GetMesh()->SetRenderCustomDepth(true);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Stat Component
	mStats = CreateDefaultSubobject<UCharacterStatComponent>(TEXT("Stats"));

	// Widget Component
	mHpBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HpBar"));
	mHpBarWidget->SetupAttachment(RootComponent);
	mHpBarWidget->SetRelativeLocation(FVector(0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));

	const static ConstructorHelpers::FClassFinder<UUserWidget>
		HpBar(TEXT("/Game/_Programming/UI/WBP_HpBar.WBP_HpBar_C"));
	if (HpBar.Class)
	{
		mHpBarWidget->SetWidgetClass(HpBar.Class);
		mHpBarWidget->SetDrawSize(FVector2D(150.f, 15.f));
		mHpBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
		mHpBarWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}  

void ACharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	mHpBarWidget->InitWidget();
	UHpBarWidget* HpBar = Cast<UHpBarWidget>(mHpBarWidget->GetUserWidgetObject());
	if (HpBar)
	{
		HpBar->BindHp(mStats);
		mHpBarWidget->SetHiddenInGame(true);
	}
	mStats->OnHpZero.AddUObject(this, &ACharacterBase::Die);
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ACharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	mHpBarWidget->SetHiddenInGame(false);
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, DamageAmount]()
		{
			mStats->ApplyDamage(DamageAmount);
		}, 0.1f, false);
	return Damage;
}

void ACharacterBase::SetCollisionEnable(bool Enable)
{
}

void ACharacterBase::SetHpBarVisible(bool Enable)
{
	mHpBarWidget->SetHiddenInGame(!Enable);
}

void ACharacterBase::Die()
{
	mHpBarWidget->SetHiddenInGame(true);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("IgnoreOnlyPawn"));
}

