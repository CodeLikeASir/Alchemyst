// Copyright Epic Games, Inc. All Rights Reserved.

#include "AlchemystGameMode.h"
#include "AlchemystPlayerController.h"
#include "AlchemystCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAlchemystGameMode::AAlchemystGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AAlchemystPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}