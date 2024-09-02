// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MyPlayerController.h"
#include "UI/HUDWidget.h"

AMyPlayerController::AMyPlayerController()
{
	static ConstructorHelpers::FClassFinder<UHUDWidget>
		HUDWidget(TEXT("/Game/_Programming/UI/WBP_HUD.WBP_HUD_C"));
	if (HUDWidget.Succeeded())
	{
		mHUDWidgetClass = HUDWidget.Class;
	}
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//FInputModeGameOnly GameOnlyInputMode;
	//SetInputMode(GameOnlyInputMode);

	mHUDWidget = CreateWidget<UHUDWidget>(this, mHUDWidgetClass);
	if (mHUDWidget)
	{
		mHUDWidget->AddToViewport();
	}

}
