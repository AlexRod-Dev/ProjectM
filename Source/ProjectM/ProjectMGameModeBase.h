

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CharacterController.h"
#include "Net/UnrealNetwork.h"
#include "EnemyBase.h"
#include "PlayerBase.h"

#include "ProjectMGameModeBase.generated.h"


UCLASS()
class PROJECTM_API AProjectMGameModeBase : public AGameModeBase
{
	GENERATED_BODY()


	
public:
	AProjectMGameModeBase();

	UPROPERTY(EditAnywhere, Category = "Spawn")
	TSubclassOf<class AEnemyBase>EnemyBlueprint;

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;


protected:


 virtual void HandleStartingNewPlayer_Implementation(APlayerController* _newPlayer) override;


public:
	void Respawn(ACharacterController* _playerController);

	void CheckEnemyAlive();
		
	float _enemyTimer;

	int32 _enemyAlive;


};
