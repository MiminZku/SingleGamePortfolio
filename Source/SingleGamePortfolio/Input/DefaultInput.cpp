// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultInput.h"
#include "InputMappingContext.h"
#include "InputAction.h"

UDefaultInput::UDefaultInput()
{
	{
		static ConstructorHelpers::FObjectFinder<UInputMappingContext>
			Asset(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/_Programming/Input/DefaultInput/IMC_Default.IMC_Default'"));
		if (Asset.Succeeded())
		{
			mMappingContext = Asset.Object;
		}
	}
	{
		static ConstructorHelpers::FObjectFinder<UInputAction>
			Asset(TEXT("/Script/EnhancedInput.InputAction'/Game/_Programming/Input/DefaultInput/IA_Move.IA_Move'"));
		if (Asset.Succeeded())
		{
			mMoveAction = Asset.Object;
		}
	}
	{
		static ConstructorHelpers::FObjectFinder<UInputAction>
			Asset(TEXT("/Script/EnhancedInput.InputAction'/Game/_Programming/Input/DefaultInput/IA_Look.IA_Look'"));
		if (Asset.Succeeded())
		{
			mLookAction = Asset.Object;
		}
	}
	{
		static ConstructorHelpers::FObjectFinder<UInputAction>
			Asset(TEXT("/Script/EnhancedInput.InputAction'/Game/_Programming/Input/DefaultInput/IA_Jump.IA_Jump'"));
		if (Asset.Succeeded())
		{
			mJumpAction = Asset.Object;
		}
	}
	{
		static ConstructorHelpers::FObjectFinder<UInputAction>
			Asset(TEXT("/Script/EnhancedInput.InputAction'/Game/_Programming/Input/DefaultInput/IA_WeakAttack.IA_WeakAttack'"));
		if (Asset.Succeeded())
		{
			mWeakAttackAction = Asset.Object;
		}
	}
	{
		static ConstructorHelpers::FObjectFinder<UInputAction>
			Asset(TEXT("/Script/EnhancedInput.InputAction'/Game/_Programming/Input/DefaultInput/IA_WeakAttack.IA_WeakAttack'"));
		if (Asset.Succeeded())
		{
			mStrongAttackAction = Asset.Object;
		}
	}
	{
		static ConstructorHelpers::FObjectFinder<UInputAction>
			Asset(TEXT("/Script/EnhancedInput.InputAction'/Game/_Programming/Input/DefaultInput/IA_Dash.IA_Dash'"));
		if (Asset.Succeeded())
		{
			mDashAction = Asset.Object;
		}
	}
	{
		static ConstructorHelpers::FObjectFinder<UInputAction>
			Asset(TEXT("/Script/EnhancedInput.InputAction'/Game/_Programming/Input/DefaultInput/IA_Arm_Unarm.IA_Arm_Unarm'"));
		if (Asset.Succeeded())
		{
			mArmUnarmAction = Asset.Object;
		}
	}
}
