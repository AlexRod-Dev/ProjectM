// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Net/UnrealNetwork.h"
#include "Blueprint/UserWidget.h"
#include "Player/CharacterController.h"
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

	void UpdatePlayersAlive(int32 _players);

	UFUNCTION(Server,Reliable)
	void CheckForPlayersAlive();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ShowEndGameWidget();

	UFUNCTION(NetMulticast, Reliable)
	void MultiShowEndGameWidget();
	
	UPROPERTY(BlueprintReadOnly, Replicated)
	int32 _playersAlive;
	
	UFUNCTION(BlueprintCallable)
	int32 GetPlayersAlive() {return _playersAlive;}
	
	TSubclassOf<class UUserWidget> EndGameWidgetClass;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
