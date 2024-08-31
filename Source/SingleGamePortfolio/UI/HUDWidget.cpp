// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUDWidget.h"
#include "UI/ProgressBarWidget.h"
#include "Character/PlayerCharacter.h"
#include "Components/TextBlock.h"
#include "CharacterStat/CharacterStatComponent.h"

UHUDWidget::UHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	mLevelText = Cast<UTextBlock>(GetWidgetFromName(TEXT("LevelText")));
	ensure(mLevelText);

	mHpBar = Cast<UProgressBarWidget>(GetWidgetFromName(TEXT("HpBarWidget")));
	ensure(mHpBar);

	mMpBar = Cast<UProgressBarWidget>(GetWidgetFromName(TEXT("MpBarWidget")));
	ensure(mHpBar);

	mExpBar = Cast<UProgressBarWidget>(GetWidgetFromName(TEXT("ExpBarWidget")));
	ensure(mHpBar);

	APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwningPlayerPawn());
	if (Player)
	{
		Player->SetupHUDWidget(this);
		Player->GetStatComponent()->OnLevelUp.AddUObject(this, &UHUDWidget::UpdateLevelText);
	}
}

void UHUDWidget::BindStats(UCharacterStatComponent* StatComp)
{
	mHpBar->BindStat(StatComp, TEXT("Hp"));
	mMpBar->BindStat(StatComp, TEXT("Mp"));
	mExpBar->BindStat(StatComp, TEXT("Exp"));
	mHpBar->UpdateProgressBar();
	mMpBar->UpdateProgressBar();
	mExpBar->UpdateProgressBar();
	UpdateLevelText();
}

void UHUDWidget::UpdateLevelText()
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwningPlayerPawn());
	if (Player)
	{
		mLevelText->SetText(FText::FromString(FString::Printf(TEXT("%d"),
			Player->GetStatComponent()->GetLevel())));
	}
}
