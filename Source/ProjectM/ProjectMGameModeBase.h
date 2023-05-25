

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CharacterController.h"
#include "Net/UnrealNetwork.h"
#include "EnemyBase.h"
#include "PlayerBase.h"
#include "ProjectMGameModeBase.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSpawnEnemy);

UCLASS()
class PROJECTM_API AProjectMGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	
public:
	AProjectMGameModeBase();
	
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;


	// UPROPERTY(BlueprintAssignable, Category = "MyEvent")
	// FSpawnEnemy fSpawnEnemy;

protected:


	virtual void HandleStartingNewPlayer_Implementation(APlayerController* _newPlayer);

public:
	void Respawn(ACharacterController* _playerController);
		
	float _enemyTimer;




};
