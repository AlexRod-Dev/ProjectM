// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

UCLASS()
class PROJECTM_API AEnemySpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemySpawner();

	UPROPERTY(EditAnywhere, Category = "Spawn")
	TSubclassOf<class AEnemyBase> BasicEnemyBlueprint;

	UPROPERTY(EditDefaultsOnly, Replicated)
	int32 _baseEnemies;

	UPROPERTY(EditDefaultsOnly, Replicated)
	int32 _incrementFactor;

	UPROPERTY(Replicated)
	int32 _waveNumber;

	int32 _enemiesToSpawn;

	FVector _enemySpawnLocation;
	
	UPROPERTY(Replicated)
	TArray<AEnemyBase*> _activeEnemies;;

	FTimerHandle SpawnTimerHandle;
	float _spawnDelay;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SpawnWave();
	void Server_SpawnWave_Implementation();
	bool Server_SpawnWave_Validate();

	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_IncrementWaveNumber();
	void Server_IncrementWaveNumber_Implementation();
	bool Server_IncrementWaveNumber_Validate();

	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_CheckEnemiesAlive();
	void Server_CheckEnemiesAlive_Implementation();
	bool Server_CheckEnemiesAlive_Validate();

	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:

	UFUNCTION()
	void SpawnEnemyWithDelay();
	
	
};
