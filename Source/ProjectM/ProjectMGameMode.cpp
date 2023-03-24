// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectMGameMode.h"
#include "ProjectMCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProjectMGameMode::AProjectMGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> TopDownCharacterClass(TEXT("/Game/Blueprints/TopDownCharacter"));
	if (TopDownCharacterClass.Class != NULL)
	{
		DefaultPawnClass = TopDownCharacterClass.Class;
	}
	else {
		DefaultPawnClass = TopDownCharacterClass.Class;
	}
}
