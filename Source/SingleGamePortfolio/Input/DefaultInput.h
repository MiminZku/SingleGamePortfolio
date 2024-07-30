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

private:
	UInputMappingContext* mMappingContext = nullptr;

	UInputAction* mMoveAction = nullptr;
	UInputAction* mLookAction = nullptr;
	UInputAction* mJumpAction = nullptr;
	UInputAction* mRunAction = nullptr;
	UInputAction* mWeakAttackAction = nullptr;
	UInputAction* mStrongAttackAction = nullptr;
	UInputAction* mDashAction = nullptr;
	UInputAction* mArmUnarmAction = nullptr;
};
