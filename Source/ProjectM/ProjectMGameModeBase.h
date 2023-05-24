

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CharacterController.h"
#include "Net/UnrealNetwork.h"
#include "EnemyBase.h"
#include "PlayerBase.h"

#include "ProjectMGameModeBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSpawnEnemy);

UCLASS()
class PROJECTM_API AProjectMGameModeBase : public AGameModeBase
{
	GENERATED_BODY()


	
public:
	AProjectMGameModeBase();


	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	/** Property replication */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintAssignable, Category = "MyEvent")
	FSpawnEnemy fSpawnEnemy;

	

protected:


 virtual void HandleStartingNewPlayer_Implementation(APlayerController* _newPlayer) override;


public:
	void Respawn(ACharacterController* _playerController);

	void CheckEnemyAlive();
		
	float _enemyTimer;

	UPROPERTY(BlueprintReadWrite,Replicated, Category="spawn")
	int32 _enemyAlive;

	UPROPERTY(BlueprintReadWrite,Replicated, Category="Spawn")
	int32 _waveCount;

	//Getter for Current Enemies Alive
	UFUNCTION(BlueprintPure, Category = "Spawn")
	FORCEINLINE float GetCurrentEnemyAlive() const { return _enemyAlive; }

	//Getter for Current Wave
	UFUNCTION(BlueprintPure, Category = "Spawn")
	FORCEINLINE float GetCurrentWave() const { return _waveCount; }

	UFUNCTION(Server,Reliable, WithValidation)
	void UpdateEnemiesAlive(int32 _enemies);
	
};
