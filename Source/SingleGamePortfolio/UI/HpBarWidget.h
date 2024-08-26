// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class SINGLEGAMEPORTFOLIO_API UHpBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UHpBarWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	void BindHp(class UCharacterStatComponent* StatComp);
	void UpdateHpBar();
	
	void SetProgressBarColor(FLinearColor NewColor);

protected:
	TWeakObjectPtr<class UCharacterStatComponent> mCurrentStatComp;

	UPROPERTY()
	TObjectPtr<class UProgressBar> mHpBarWidget;

private:
};
