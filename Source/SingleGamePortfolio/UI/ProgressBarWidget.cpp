// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ProgressBarWidget.h"
#include "Components/ProgressBar.h"
#include "CharacterStat/CharacterStatComponent.h"
#include "Components/TextBlock.h"

UProgressBarWidget::UProgressBarWidget(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{

}

void UProgressBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	mProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("ProgressBar")));
	ensure(mProgressBar);

	mCurValueText = Cast<UTextBlock>(GetWidgetFromName(TEXT("CurValue")));
	ensure(mCurValueText);

	mMaxValueText = Cast<UTextBlock>(GetWidgetFromName(TEXT("MaxValue")));
	ensure(mMaxValueText);
}

void UProgressBarWidget::BindStat(UCharacterStatComponent* StatComp, const FName& StatName)
{
	mCurrentStatComp = StatComp;
	mBindStatName = StatName;
	if (TEXT("Hp") == StatName)	StatComp->OnHpChanged.AddUObject(this, &UProgressBarWidget::UpdateProgressBar);
	if (TEXT("Mp") == StatName)	StatComp->OnMpChanged.AddUObject(this, &UProgressBarWidget::UpdateProgressBar);
	if (TEXT("Exp") == StatName)StatComp->OnExpChanged.AddUObject(this, &UProgressBarWidget::UpdateProgressBar);
}

void UProgressBarWidget::UpdateProgressBar()
{
	if (mProgressBar)
	{
		if (mCurrentStatComp.IsValid())
		{
			mProgressBar->SetPercent(mCurrentStatComp->GetStatRatio(mBindStatName));
			mCurValueText->SetText(FText::FromString(
				FString::Printf(TEXT("%.f"), mCurrentStatComp->GetCurrentStat(mBindStatName))));
			mMaxValueText->SetText(FText::FromString(
				FString::Printf(TEXT("%.f"), mCurrentStatComp->GetMaxStat(mBindStatName))));
		}
	}
}

void UProgressBarWidget::SetProgressBarColor(FLinearColor NewColor)
{
	if (mProgressBar)
	{
		mProgressBar->SetFillColorAndOpacity(NewColor);
	}
}
