// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Net/UnrealNetwork.h"
#include "ProjectMGameStateBase.generated.h"


UCLASS()
class PROJECTM_API AProjectMGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	AProjectMGameStateBase();

	UPROPERTY(Replicated)
	int32 _enemiesAlive;

	UPROPERTY(Replicated)
	int32 _currentWave;

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentWave() { return _currentWave; }

	UFUNCTION(BlueprintCallable)
	int32 GetEnemiesAlive() { return _enemiesAlive; }

	UFUNCTION(BlueprintCallable, Category = "Game State")
	void UpdateEnemiesAlive(int32 _enemies);


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
