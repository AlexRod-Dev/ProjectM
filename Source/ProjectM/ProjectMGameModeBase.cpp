// Copyright Epic Games, Inc. All Rights Reserved.


#include "ProjectMGameModeBase.h"
#include "CharacterController.h"
#include "UObject/SoftObjectPtr.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Containers/Array.h"
#include "Kismet/KismetMathLibrary.h"



AProjectMGameModeBase::AProjectMGameModeBase()
{
	//use our custom PlayerController class
	PlayerControllerClass = ACharacterController::StaticClass();


	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBP(TEXT("/Game/Blueprints/BP_PlayerBase"));

	if(PlayerPawnBP.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBP.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<ACharacterController> PlayerController(TEXT("/Game/Blueprints/BP_CharacterController"));
	if(PlayerController.Class != NULL)
	{
		PlayerControllerClass = PlayerController.Class;
	}

	PrimaryActorTick.bCanEverTick = true;
}

void AProjectMGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	

}

void AProjectMGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	_enemyTimer -= DeltaTime;

	if(_enemyTimer < 0.0f)
	{
		_enemyTimer = 5.0f;

		UWorld* _world = GetWorld();

		if(_world)
		{

			//Enemy Spawned (Change static location to  dynamic
			FVector _enemySpawnLocation = FVector(-2100.0f, 70.0f, 0.0f);
			_world->SpawnActor<AEnemyBase>(EnemyBlueprint, _enemySpawnLocation, FRotator::ZeroRotator);
			

		}
	}
	


}


void AProjectMGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* _newPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(_newPlayer);

	
	ACharacterController* TempPlayerToCheck = Cast<ACharacterController>(_newPlayer->GetPawn());

	
	if (TempPlayerToCheck)
		return;
		

	if (_newPlayer->GetPawn())
	{
		_newPlayer->GetPawn()->Destroy();
	}

	FTransform tSpawnTransform;
	TArray<AActor*> _foundEntries;
	int32 _players = _foundEntries.Num();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacterController::StaticClass(), _foundEntries);

	if(_foundEntries.Num() > 0)
	{
		tSpawnTransform = _foundEntries[0]->GetActorTransform();
	}
	else
	{
		TArray<AActor*> _foundPlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), _foundPlayerStarts);
		tSpawnTransform = _foundPlayerStarts[0]->GetActorTransform();
	}

	float _TempX = UKismetMathLibrary::RandomBool() ? UKismetMathLibrary::RandomFloatInRange(-50.f, -150.0f) : UKismetMathLibrary::RandomFloatInRange(50.f, 150.0f);
	float _TempY = UKismetMathLibrary::RandomFloatInRange(-50.0f, -150.0f);

	tSpawnTransform.SetLocation((tSpawnTransform.GetLocation() + FVector(_TempX, _TempY, 0.0f)));

	FActorSpawnParameters _spawnInfo;
	_spawnInfo.Owner = _newPlayer;
	_spawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	FVector _tempLoc = tSpawnTransform.GetLocation();
	FRotator _tempRot = tSpawnTransform.GetRotation().Rotator();

	APlayerBase* _tempPlayer = Cast<APlayerBase>(GetWorld()->SpawnActor(DefaultPawnClass, &_tempLoc, &_tempRot, _spawnInfo));
	
	_newPlayer->Possess(_tempPlayer);
}

void AProjectMGameModeBase::Respawn(ACharacterController* _playerController)
{
	FTransform tSpawnTransform;
	
	UWorld* _world = GetWorld();
	if(_world != nullptr)
	{
		for(FConstPlayerControllerIterator it = _world->GetPlayerControllerIterator(); it; ++it)
		{
			ACharacterController* _players = Cast<ACharacterController>(it->Get());
			if(_players->GetIsAlive())
			{
				tSpawnTransform = _players->GetPawn()->GetActorTransform();
				FVector Location = tSpawnTransform.GetLocation();

				if(APawn* Pawn = GetWorld()->SpawnActor<APawn>(DefaultPawnClass, Location, FRotator::ZeroRotator))
				{
				 	_playerController->Possess(Pawn);
				 	_playerController->bIsAlive = true;
				 	break;
				}
			}
			else
			{
				//means everyone is dead so end the game
				UE_LOG(LogTemp,Warning, TEXT("You all ded"));
			}
		}
	}
}

void AProjectMGameModeBase::CheckEnemyAlive()
{
}