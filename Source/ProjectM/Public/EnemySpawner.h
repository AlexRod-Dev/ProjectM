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


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	int32 _enemiesToSpawn;

	int32 _initialCountDown;

	int32 _enemyIncrement;

	UFUNCTION(BlueprintCallable, Category="Spawn")
	void SpawnEnemy();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
