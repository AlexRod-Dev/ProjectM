// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "../ProjectMGameModeBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
_enemyIncrement = _enemiesToSpawn;
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority())
	{
		AProjectMGameModeBase* _gameMode = Cast<AProjectMGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

		if(_gameMode != nullptr)
		{
			_gameMode->fSpawnEnemy.AddDynamic(this, &AEnemySpawner::SpawnEnemy);
			
		}
	}

	
}

void AEnemySpawner::SpawnEnemy()
{
	UWorld* _world = GetWorld();
	FVector _enemySpawnLocation = GetActorLocation();
	
	for(int i = 0; i < (_enemiesToSpawn-1); i++)
	{
			if(_world)
			{
				_world->SpawnActor<AEnemyBase>(BasicEnemyBlueprint, _enemySpawnLocation, FRotator::ZeroRotator);
			}
	}

	AProjectMGameModeBase* _gameMode = Cast<AProjectMGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	TArray<AActor*> _foundEntries;
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacterController::StaticClass(), _foundEntries);
	int32 _players = _foundEntries.Num();

	_enemiesToSpawn = ++_enemyIncrement + _players;
	_gameMode->_waveCount++;
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

