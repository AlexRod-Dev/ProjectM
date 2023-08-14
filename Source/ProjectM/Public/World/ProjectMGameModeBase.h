#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Player/CharacterController.h"
#include "Net/UnrealNetwork.h"
#include "Enemies/EnemyBase.h"
#include "Player/PlayerBase.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
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

protected:
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* _newPlayer) override;

	
	
public:
	void Respawn(ACharacterController* _playerController);

	float _enemyTimer;

private:

};
