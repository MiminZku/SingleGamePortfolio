// Copyright Epic Games, Inc. All Rights Reserved.

#include "SingleGamePortfolioGameMode.h"
#include "SingleGamePortfolioCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Character/GreatSwordPlayer.h"
#include "Character/MyPlayerController.h"

ASingleGamePortfolioGameMode::ASingleGamePortfolioGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn>
		PlayerPawnBPClass(TEXT("/Game/_Programming/Blueprints/BP_Hercules.BP_Hercules_C"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	//DefaultPawnClass = AGreatSwordPlayer::StaticClass();

	PlayerControllerClass = AMyPlayerController::StaticClass();
}
