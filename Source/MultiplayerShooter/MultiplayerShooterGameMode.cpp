// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiplayerShooterGameMode.h"
#include "MultiplayerShooterCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMultiplayerShooterGameMode::AMultiplayerShooterGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
