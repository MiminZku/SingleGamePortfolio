// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HpBarWidget.h"
#include "Components/ProgressBar.h"
#include "CharacterStat/CharacterStatComponent.h"

UHpBarWidget::UHpBarWidget(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
}

void UHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	mHpBarWidget = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HpBar")));
	ensure(mHpBarWidget);
}

void UHpBarWidget::BindHp(UCharacterStatComponent* StatComp)
{
	mCurrentStatComp = StatComp;
	StatComp->OnHpChanged.AddUObject(this, &UHpBarWidget::UpdateHpBar);
}

void UHpBarWidget::UpdateHpBar()
{
	if (mHpBarWidget)
	{
		if (mCurrentStatComp.IsValid())
		{
			mHpBarWidget->SetPercent(mCurrentStatComp->GetHpRatio());
		}
	}
}

void UHpBarWidget::SetProgressBarColor(FLinearColor NewColor)
{
	if (mHpBarWidget)
	{
		mHpBarWidget->SetFillColorAndOpacity(NewColor);
	}
}
