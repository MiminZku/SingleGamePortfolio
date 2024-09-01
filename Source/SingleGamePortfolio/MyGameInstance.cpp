// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

UMyGameInstance::UMyGameInstance()
{
	{
		static ConstructorHelpers::FObjectFinder<UDataTable>
			DT(TEXT("/Script/Engine.DataTable'/Game/_Programming/Character/DT_PlayerAnim.DT_PlayerAnim'"));
		if (DT.Succeeded())
		{
			mPlayerAnims = DT.Object;
		}
	}
	{
		static ConstructorHelpers::FObjectFinder<UDataTable>
			DT(TEXT("/Script/Engine.DataTable'/Game/_Programming/Character/Enemy/DT_MonsterAnim.DT_MonsterAnim'"));
		if (DT.Succeeded())
		{
			mMonsterAnims = DT.Object;
		}
	}
	{
		static ConstructorHelpers::FObjectFinder<UDataTable>
			DT(TEXT("/Script/Engine.DataTable'/Game/_Programming/Character/DT_CharacterStat.DT_CharacterStat'"));
		if (DT.Succeeded())
		{
			mCharactetStats = DT.Object;
		}
	}
}

void UMyGameInstance::Init()
{
	Super::Init();

	
}

FPlayerAnimData* UMyGameInstance::GetPlayerAnimData(const FName& CharacterName) const
{
	return mPlayerAnims->FindRow<FPlayerAnimData>(CharacterName, TEXT(""));
}

FMonsterAnimData* UMyGameInstance::GetMonsterAnimData(const FName& MonsterName) const
{
	return  mMonsterAnims->FindRow<FMonsterAnimData>(MonsterName, TEXT(""));
}

FCharacterStatData* UMyGameInstance::GetCharacterStatData(const FName& Level) const
{
	return mCharactetStats->FindRow<FCharacterStatData>(Level, TEXT(""));
}
