// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class SINGLEGAMEPORTFOLIO_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UHUDWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	void BindStats(class UCharacterStatComponent* StatComp);
	UFUNCTION()
	void UpdateLevelText();

protected:
	UPROPERTY()
	TObjectPtr<class UTextBlock> mLevelText;
	
	UPROPERTY()
	TObjectPtr<class UProgressBarWidget> mHpBar;

	UPROPERTY()
	TObjectPtr<class UProgressBarWidget> mMpBar;

	UPROPERTY()
	TObjectPtr<class UProgressBarWidget> mExpBar;
};
