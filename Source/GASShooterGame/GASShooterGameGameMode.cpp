// Copyright Epic Games, Inc. All Rights Reserved.

#include "GASShooterGameGameMode.h"

#include "AbilitySystemGlobals.h"
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

void AGASShooterGameGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	UAbilitySystemGlobals::Get().InitGlobalData();
}
