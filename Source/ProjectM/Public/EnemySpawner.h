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
	TSubclassOf<class AEnemyBase>BasicEnemyBlueprint;
	
	UPROPERTY(EditDefaultsOnly, Replicated)
	int32 _baseEnemies;
	
	UPROPERTY(EditDefaultsOnly, Replicated)
	int32 _incrementFactor;

	UPROPERTY(Replicated)
	int32 _waveNumber;
	
	UPROPERTY(Replicated)
	TArray<AEnemyBase*> _activeEnemies;;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	

	UFUNCTION(Server, Reliable, WithValidation)
	void SpawnWave();
	
	UFUNCTION(Server, Reliable, WithValidation)
	void IncrementWaveNumber();

	UFUNCTION(Server, Reliable, WithValidation)
	void CheckEnemiesAlive();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:


	void SpawnWave_Implementation();


	bool SpawnWave_Validate();


	void IncrementWaveNumber_Implementation();

	
	bool IncrementWaveNumber_Validate();


	void CheckEnemiesAlive_Implementation();


	bool CheckEnemiesAlive_Validate();
	
};
