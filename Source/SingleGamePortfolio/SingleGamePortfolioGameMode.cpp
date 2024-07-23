// Copyright Epic Games, Inc. All Rights Reserved.

#include "SingleGamePortfolioGameMode.h"
#include "SingleGamePortfolioCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASingleGamePortfolioGameMode::ASingleGamePortfolioGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
