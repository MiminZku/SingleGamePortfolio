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

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);


	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Stat Component
	mStats = CreateDefaultSubobject<UCharacterStatComponent>(TEXT("Stats"));

	// Widget Component
	mHpBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HpBar"));
	mHpBar->SetupAttachment(GetMesh());
	mHpBar->SetRelativeLocation(FVector(0.f, 0.f, 170.f));

	const static ConstructorHelpers::FClassFinder<UUserWidget>
		HpBar(TEXT("/Game/_Programming/UI/WBP_HpBar.WBP_HpBar_C"));
	if (HpBar.Class)
	{
		mHpBar->SetWidgetClass(HpBar.Class);
		mHpBar->SetDrawSize(FVector2D(150.f, 15.f));
		mHpBar->SetWidgetSpace(EWidgetSpace::Screen);
		mHpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	 }
}

void ACharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	mHpBar->InitWidget();
	UHpBarWidget* HpWidget = Cast<UHpBarWidget>(mHpBar->GetUserWidgetObject());
	if (HpWidget)
	{
		HpWidget->BindHp(mStats);
		mHpBar->SetVisibility(false);
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
	mHpBar->SetVisibility(true);
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, DamageAmount]()
		{
			mStats->ApplyDamage(DamageAmount);
		}, 0.1f, false);
	return Damage;
}

void ACharacterBase::Die()
{
	mHpBar->SetHiddenInGame(true);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

