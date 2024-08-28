// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ProgressBarWidget.h"
#include "Components/ProgressBar.h"
#include "CharacterStat/CharacterStatComponent.h"

UProgressBarWidget::UProgressBarWidget(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
}

void UProgressBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	mProgressBarWidget = Cast<UProgressBar>(GetWidgetFromName(TEXT("ProgressBar")));
	ensure(mProgressBarWidget);
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
	if (mProgressBarWidget)
	{
		if (mCurrentStatComp.IsValid())
		{
			mProgressBarWidget->SetPercent(mCurrentStatComp->GetStatRatio(mBindStatName));
		}
	}
}

void UProgressBarWidget::SetProgressBarColor(FLinearColor NewColor)
{
	if (mProgressBarWidget)
	{
		mProgressBarWidget->SetFillColorAndOpacity(NewColor);
	}
}
