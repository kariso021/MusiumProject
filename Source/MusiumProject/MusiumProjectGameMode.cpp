// Copyright Epic Games, Inc. All Rights Reserved.

#include "MusiumProjectGameMode.h"
#include "MusiumProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMusiumProjectGameMode::AMusiumProjectGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
