// Copyright Epic Games, Inc. All Rights Reserved.

#include "GASShooterGameGameMode.h"
#include "GASShooterGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGASShooterGameGameMode::AGASShooterGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
