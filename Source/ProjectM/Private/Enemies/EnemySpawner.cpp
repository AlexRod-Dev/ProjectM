// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/EnemySpawner.h"
#include "Net/UnrealNetwork.h"
#include "World/ProjectMGameStateBase.h"
#include "Enemies/EnemyBase.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Enables Replication
	bReplicates = true;

	//Base enemies to spawn
	_baseEnemies = 2;
	
	_incrementFactor = 2;
	_waveNumber = 1;

	//enemy spawner delay
	_spawnDelay = 1.0f;
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		Server_SpawnWave();
	}
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		Server_CheckEnemiesAlive();
	}
}


void AEnemySpawner::Server_SpawnWave_Implementation()
{
	//the Spawn Location is this actor Location
	_enemySpawnLocation = GetActorLocation();

	//Change increment factor or wave number to the number of online players
	_enemiesToSpawn = _baseEnemies + (_waveNumber - 1) * _incrementFactor;

	SpawnEnemyWithDelay();
}

void AEnemySpawner::SpawnEnemyWithDelay()
{
	UWorld* _world = GetWorld();

	if (_world)
	{
		FActorSpawnParameters _spawnParams;
		_spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		AEnemyBase* _spawnedEnemy = _world->SpawnActor<AEnemyBase>(BasicEnemyBlueprint, _enemySpawnLocation,
																   FRotator::ZeroRotator, _spawnParams);
		_activeEnemies.Add(_spawnedEnemy);
	}

	// Check if there are more enemies to spawn
	if (_activeEnemies.Num() < _enemiesToSpawn)
	{
	
		GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AEnemySpawner::SpawnEnemyWithDelay, _spawnDelay, false);
	}
	else
	{
		// Clear the timer handle when all enemies have been spawned
		FTimerManager& TimerManager = GetWorld()->GetTimerManager();
		TimerManager.ClearTimer(SpawnTimerHandle);
	}

 }

bool AEnemySpawner::Server_SpawnWave_Validate()
{
	return true;
}

void AEnemySpawner::Server_IncrementWaveNumber_Implementation()
{
	_waveNumber++;

	GetWorld()->GetGameState<AProjectMGameStateBase>()->_currentWave = _waveNumber;
}

bool AEnemySpawner::Server_IncrementWaveNumber_Validate()
{
	return true;
}

void AEnemySpawner::Server_CheckEnemiesAlive_Implementation()
{
	for (AEnemyBase* _enemy : _activeEnemies)
	{
		if (_enemy && _enemy->IsAlive())
		{
			return;
		}
	}
	_activeEnemies.Empty();
	Server_IncrementWaveNumber();
	Server_SpawnWave();
}

bool AEnemySpawner::Server_CheckEnemiesAlive_Validate()
{
	return true;
}

void AEnemySpawner::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEnemySpawner, _waveNumber);
	DOREPLIFETIME(AEnemySpawner, _baseEnemies);
	DOREPLIFETIME(AEnemySpawner, _incrementFactor);
	DOREPLIFETIME(AEnemySpawner, _activeEnemies);
}


