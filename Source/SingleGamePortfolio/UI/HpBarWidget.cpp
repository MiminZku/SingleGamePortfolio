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
	mHpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HpBar")));
	ensure(mHpBar);
}

void UHpBarWidget::BindHp(UCharacterStatComponent* StatComp)
{
	mCurrentStatComp = StatComp;
	StatComp->OnHpChanged.AddUObject(this, &UHpBarWidget::UpdateHpBar);
}

void UHpBarWidget::UpdateHpBar()
{
	if (mHpBar)
	{
		if (mCurrentStatComp.IsValid())
			//mHpBar->SetPercent(NewCurrentHp / mMaxHp);
			mHpBar->SetPercent(mCurrentStatComp->GetHpRatio());
	}
}
