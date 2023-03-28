// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectMGameMode.h"
#include "PlayerBase.h"
#include "UObject/ConstructorHelpers.h"

AProjectMGameMode::AProjectMGameMode()
{

	MaxPlayersPerSpawnPoint = 1;

	//// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> TopDownCharacterClass(TEXT("/Game/Blueprints/TopDownCharacter"));
	//if (TopDownCharacterClass.Class != NULL)
	//{
	//	DefaultPawnClass = TopDownCharacterClass.Class;
	//}
	//else {
	//	DefaultPawnClass = TopDownCharacterClass.Class;
	//}

}

void AProjectMGameMode(APlayerBase* NewPlayer)
{
   

    // Loop through all the multiplayer spawn points and spawn the player at the first available point
    for (ASpawnPoint* SpawnPoint : SpawnPoints)
    {
        if (SpawnPoint->CanSpawnPlayer(MaxPlayersPerSpawnPoint))
        {
            // Spawn the player character at the spawn point
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;
            SpawnParams.Instigator = Instigator;
            APlayerBase* NewCharacter = GetWorld()->SpawnActor<APlayerBase>(SpawnPoint->GetActorLocation(), FRotator::ZeroRotator, SpawnParams);

            // Set the spawn point variable in the character class
            NewCharacter->SetMultiplayerSpawnPoint(SpawnPoint);

            // Increment the player count variable in the spawn point class
            SpawnPoint->IncrementPlayerCount();

            break;
        }
    }
}
