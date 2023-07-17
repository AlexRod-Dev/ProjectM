// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/EnemyBTTask_FindClosestPlayer.h"

#include "AIController.h"
#include "Enemies/EnemyAIController.h"
#include "EngineUtils.h"
#include "NavigationSystem.h"
#include "Player/PlayerBase.h"
#include "Player/CharacterController.h"
#include "BehaviorTree/BlackboardComponent.h"

UEnemyBTTask_FindClosestPlayer::UEnemyBTTask_FindClosestPlayer()
{
	NodeName = TEXT("Find Closest Player");

	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UEnemyBTTask_FindClosestPlayer, BlackboardKey));

	//bNotifyTick = true;
}

EBTNodeResult::Type UEnemyBTTask_FindClosestPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyAIController* _aiEnemyController{(Cast<AEnemyAIController>(OwnerComp.GetAIOwner()))};
	if(_aiEnemyController == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	//OwnerComp.GetAIOwner()->SetFocus(_aiEnemyController->GetPawn());

	Delegate.BindUFunction(this, "FindClosestPlayer", _aiEnemyController);
	// Start the timer to periodically find the closest player
	_aiEnemyController->GetWorld()->GetTimerManager().SetTimer(_aiEnemyController->FindPlayerTimerHandle, Delegate, 0.2f,true);

	return EBTNodeResult::Succeeded;
}

void UEnemyBTTask_FindClosestPlayer::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                                                    EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

	AEnemyAIController* _aiEnemyController{(Cast<AEnemyAIController>(OwnerComp.GetAIOwner()))};
	if(_aiEnemyController != nullptr)
	{
		_aiEnemyController->GetWorld()->GetTimerManager().ClearTimer(_aiEnemyController->FindPlayerTimerHandle);
	}
}


void UEnemyBTTask_FindClosestPlayer::FindClosestPlayer(AEnemyAIController* _aiEnemyController)
{
	
	if (_aiEnemyController == nullptr)
	{
		return;
	}

	APawn* _aiPawn{_aiEnemyController->GetPawn()};
	if (_aiPawn == nullptr)
	{
		return;
	}

	APawn* _closestPlayer{nullptr};
	float _closestDistance{TNumericLimits<float>::Max()};

	UWorld* _world = GetWorld();
	if (_world != nullptr)
	{
		for (FConstPlayerControllerIterator it = _world->GetPlayerControllerIterator(); it; ++it)
		{
			ACharacterController* _players = Cast<ACharacterController>(it->Get());
			if (_players->GetIsAlive())
			{
				APawn* _playerPawn = _players->GetPawn();
				float _distance = FVector::Distance(_aiPawn->GetActorLocation(), _playerPawn->GetActorLocation());

				//If this pawn is closer update the location variable
				if (_distance < _closestDistance)
				{
					_closestPlayer = _playerPawn;
					_closestDistance = _distance;
				}
			}
		}
		if (_closestPlayer != nullptr)
		{
			_aiEnemyController->GetBlackboardComponent()->SetValueAsVector(
				"TargetLocation", _closestPlayer->GetActorLocation());
			
			UE_LOG(LogTemp, Warning, TEXT("Player location : %s"), *_closestPlayer->GetActorLocation().ToString());
		}
	}
}


FString UEnemyBTTask_FindClosestPlayer::GetStaticDescription() const
{
	return FString::Printf(TEXT("vector: %s"), *BlackboardKey.SelectedKeyName.ToString());
}


