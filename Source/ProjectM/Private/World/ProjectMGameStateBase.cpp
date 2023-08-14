// Fill out your copyright notice in the Description page of Project Settings.


#include "World/ProjectMGameStateBase.h"

#include "Player/CharacterController.h"


AProjectMGameStateBase::AProjectMGameStateBase()
{
	SetReplicates(true);

	_enemiesAlive = 0;
	_currentWave = 1;

	static ConstructorHelpers::FClassFinder<UUserWidget> EndGameWidget(TEXT("/Game/UI/EndGameScreen"));
	if(EndGameWidget.Class != nullptr)
	{
		EndGameWidgetClass = EndGameWidget.Class;
	}

}

void AProjectMGameStateBase::UpdateEnemiesAlive(int32 _enemies)
{
	if (HasAuthority())
	{
		_enemiesAlive += _enemies;
	}
}

void AProjectMGameStateBase::UpdatePlayersAlive(int32 _players)
{
	if(HasAuthority())
	{
		_playersAlive+=_players;
	}
}


void AProjectMGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AProjectMGameStateBase, _enemiesAlive);
	DOREPLIFETIME(AProjectMGameStateBase, _currentWave);
	DOREPLIFETIME(AProjectMGameStateBase, _playersAlive);

}


void AProjectMGameStateBase::ShowEndGameWidget()
{
	// Iterate through all player controllers
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PlayerController = It->Get();
		if (PlayerController)
		{
			ACharacterController* _playerController = Cast<ACharacterController>(PlayerController);

			UGameViewportClient* ViewportClient = _playerController->GetWorld()->GetGameViewport();

			if(ViewportClient)
			{
				ViewportClient->RemoveAllViewportWidgets();

				//Create Widget and add it to viewport
				UUserWidget* EndGameMenu  = CreateWidget<UUserWidget>(_playerController, EndGameWidgetClass);

				if(EndGameMenu)
				{
					EndGameMenu->AddToViewport();

					//Set Inputs
					FInputModeUIOnly InputModeData;
	
					InputModeData.SetWidgetToFocus(EndGameMenu->TakeWidget());
					InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	
					_playerController->SetInputMode(InputModeData);
	
					_playerController->bShowMouseCursor = true;
				
				}
			
			}
			
		}
	}
}


void AProjectMGameStateBase::CheckForPlayersAlive_Implementation()
{
	if(_playersAlive <= 0)
	{	
		// Iterate through all player controllers
		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			APlayerController* PlayerController = It->Get();
			if (PlayerController)
			{
				
			
			
			}
		}
	
		if (_playersAlive == 0)
		{
			// All players are dead, handle endgame scenario
			// ...
		}
	}
	
}


