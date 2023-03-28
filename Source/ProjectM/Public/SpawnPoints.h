// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnPoints.generated.h"

UCLASS()
class PROJECTM_API ASpawnPoints : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnPoints();

	// Returns true if the maximum number of players allowed at this point has not been reached, and false otherwise
	bool CanSpawnPlayer(int32 MaxPlayersPerSpawnPoint);

	// Increment the player count variable
	void IncrementPlayerCount();

	// Decrement the player count variable
	void DecrementPlayerCount();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when an actor begins overlapping this actor
	void NotifyActorBeginOverlap(AActor* OtherActor) override;

	// Called when an actor ends overlapping this actor
	void NotifyActorEndOverlap(AActor* OtherActor) override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	// Number of players currently spawned at this point
	int32 PlayerCount;
};
