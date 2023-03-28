// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SpawnPoints.h"
#include "ProjectMGameMode.generated.h"

UCLASS(minimalapi)
class AProjectMGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AProjectMGameMode();




private:
	// Maximum number of players allowed at each spawn point
	int32 MaxPlayersPerSpawnPoint;

	// Array of  spawn points
	TArray<ASpawnPoints*> SpawnPoints;
};



