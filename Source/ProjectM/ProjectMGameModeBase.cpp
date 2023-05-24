// Copyright Epic Games, Inc. All Rights Reserved.


#include "ProjectMGameModeBase.h"
#include "CharacterController.h"
#include "UObject/SoftObjectPtr.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Containers/Array.h"
#include "EnemySpawner.h"
#include "Kismet/KismetMathLibrary.h"



AProjectMGameModeBase::AProjectMGameModeBase()
{
	//use our custom PlayerControllesr class
	PlayerControllerClass = ACharacterController::StaticClass();

	_waveCount = -2;

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

void AProjectMGameModeBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AProjectMGameModeBase, _enemyAlive);
		DOREPLIFETIME(AProjectMGameModeBase, _waveCount);
}

void AProjectMGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckEnemyAlive();
	

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
	if(_enemyAlive >= 0)
	{

		fSpawnEnemy.Broadcast();

	}
}

void AProjectMGameModeBase::UpdateEnemiesAlive_Implementation(int32 _enemies)
{
	_enemyAlive = _enemies;
}

bool AProjectMGameModeBase::UpdateEnemiesAlive_Validate(int32 _newValue)
{
	return true;
}
