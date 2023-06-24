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

	_baseEnemies = 3;
	_incrementFactor = 2;
	_waveNumber = 1;
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SpawnWave();
	}
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		CheckEnemiesAlive();
	}
}


void AEnemySpawner::SpawnWave_Implementation()
{
	//Spawn Location is this actor Location
	FVector _enemySpawnLocation = GetActorLocation();

	//Change increment factor or wave number to the number of online players
	int32 _enemiesToSpawn = _baseEnemies + (_waveNumber - 1) * _incrementFactor;
	UE_LOG(LogTemp, Warning, TEXT("enemies to spawn : %d"), _enemiesToSpawn);
	for (int32 i = 0; i < _enemiesToSpawn; i++)
	{
		UWorld* _world = GetWorld();

		if (_world)
		{
			FActorSpawnParameters _spawnParams;
			_spawnParams.SpawnCollisionHandlingOverride =
				ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			AEnemyBase* _spawnedEnemy = _world->SpawnActor<AEnemyBase>(BasicEnemyBlueprint, _enemySpawnLocation,
			                                                           FRotator::ZeroRotator, _spawnParams);
			_activeEnemies.Add(_spawnedEnemy);
		}
	}
}

bool AEnemySpawner::SpawnWave_Validate()
{
	return true;
}

void AEnemySpawner::IncrementWaveNumber_Implementation()
{
	_waveNumber++;

	GetWorld()->GetGameState<AProjectMGameStateBase>()->_currentWave = _waveNumber;
}

bool AEnemySpawner::IncrementWaveNumber_Validate()
{
	return true;
}

void AEnemySpawner::CheckEnemiesAlive_Implementation()
{
	for (AEnemyBase* _enemy : _activeEnemies)
	{
		if (_enemy && _enemy->IsAlive())
		{
			return;
		}
	}
	_activeEnemies.Empty();
	IncrementWaveNumber();
	SpawnWave();
}

bool AEnemySpawner::CheckEnemiesAlive_Validate()
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
