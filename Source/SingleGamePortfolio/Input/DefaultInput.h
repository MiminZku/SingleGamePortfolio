// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DefaultInput.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class SINGLEGAMEPORTFOLIO_API UDefaultInput : public UObject
{
	GENERATED_BODY()
	
public:
	UDefaultInput();

public:
	UInputMappingContext* GetInputMappingContext() const { return mMappingContext; }
	UInputAction* GetMoveInputAction() const { return mMoveAction; }
	UInputAction* GetLookInputAction() const { return mLookAction; }
	UInputAction* GetJumpInputAction() const { return mJumpAction; }
	UInputAction* GetRunInputAction() const { return mRunAction; }
	UInputAction* GetWeakAttackInputAction() const { return mWeakAttackAction; }
	UInputAction* GetStrongAttackInpuAction() const { return mStrongAttackAction; }
	UInputAction* GetDashInputAction() const { return mDashAction; }
	UInputAction* GetArmUnarmAction() const { return mArmUnarmAction; }
	UInputAction* GetLockOnAction() const { return mLockOnAction; }

private:
	UPROPERTY()
	UInputMappingContext* mMappingContext = nullptr;

	UPROPERTY()
	UInputAction* mMoveAction = nullptr;
	UPROPERTY()
	UInputAction* mLookAction = nullptr;
	UPROPERTY()
	UInputAction* mJumpAction = nullptr;
	UPROPERTY()
	UInputAction* mRunAction = nullptr;
	UPROPERTY()
	UInputAction* mWeakAttackAction = nullptr;
	UPROPERTY()
	UInputAction* mStrongAttackAction = nullptr;
	UPROPERTY()
	UInputAction* mDashAction = nullptr;
	UPROPERTY()
	UInputAction* mArmUnarmAction = nullptr;
	UPROPERTY()
	UInputAction* mLockOnAction = nullptr;
};
