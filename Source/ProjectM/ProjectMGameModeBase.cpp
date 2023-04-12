// Copyright Epic Games, Inc. All Rights Reserved.


#include "ProjectMGameModeBase.h"
#include "CharacterController.h"
#include "PlayerBase.h"
#include "UObject/ConstructorHelpers.h"


AProjectMGameModeBase::AProjectMGameModeBase()
{
	//use our custom PlayerController class
	PlayerControllerClass = APlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBP(TEXT("/Game/Blueprints/BP_PlayerBase"));
	if(PlayerPawnBP.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBP.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerController(TEXT("/Game/Blueprints/BP_CharacterController"));
	if(PlayerController.Class != NULL)
	{
		PlayerControllerClass = PlayerController.Class;
	}
}
