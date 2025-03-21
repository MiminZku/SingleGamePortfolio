// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProgressBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class SINGLEGAMEPORTFOLIO_API UProgressBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UProgressBarWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	void BindStat(class UCharacterStatComponent* StatComp, const FName& StatName);
	void UpdateProgressBar();
	
	void SetProgressBarColor(FLinearColor NewColor);

protected:
	UPROPERTY()
	TWeakObjectPtr<class UCharacterStatComponent> mCurrentStatComp;

	UPROPERTY()
	TObjectPtr<class UProgressBar> mProgressBar;

	UPROPERTY()
	TObjectPtr<class UTextBlock> mCurValueText;

	UPROPERTY()
	TObjectPtr<class UTextBlock> mMaxValueText;

private:
	FName mBindStatName;
};
