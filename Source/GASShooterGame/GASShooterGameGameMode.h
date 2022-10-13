// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GASShooterGameGameMode.generated.h"

UCLASS(minimalapi)
class AGASShooterGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGASShooterGameGameMode();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
};



