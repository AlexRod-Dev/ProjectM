// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/EnemyBTTask_ChasePlayer.h"

#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UEnemyBTTask_ChasePlayer::UEnemyBTTask_ChasePlayer()
{
	NodeName = TEXT("Chase Player");
	
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UEnemyBTTask_ChasePlayer, BlackboardKey));

	bNotifyTick = true;
}

EBTNodeResult::Type UEnemyBTTask_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyAIController* _aiEnemyController {(Cast<AEnemyAIController>(OwnerComp.GetAIOwner()))};
	if(_aiEnemyController == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	APawn* _aiPawn = _aiEnemyController->GetPawn();
	if(_aiPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	UBlackboardComponent* _bbComponent = OwnerComp.GetBlackboardComponent();
	if(_bbComponent == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	FVector _targetLocation = _bbComponent->GetValueAsVector("TargetLocation");
	if(!_targetLocation.IsNearlyZero())
	{
		//Move AI to initial Target Location
		_aiEnemyController->MoveToLocation(_targetLocation);
		_previousTargetLocation = _targetLocation;
	}

	return EBTNodeResult::InProgress;
}

void UEnemyBTTask_ChasePlayer::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AEnemyAIController* _aiEnemyController {(Cast<AEnemyAIController>(OwnerComp.GetAIOwner()))};
	if(_aiEnemyController == nullptr)
	{
		FinishLatentTask(OwnerComp,EBTNodeResult::Failed);
		return;
	}

	APawn* _aiPawn = _aiEnemyController->GetPawn();
	if(_aiPawn == nullptr)
	{
		
		FinishLatentTask(OwnerComp,EBTNodeResult::Failed);
		return;
	}
	
	UBlackboardComponent* _bbComponent = OwnerComp.GetBlackboardComponent();
	if(_bbComponent == nullptr)
	{
		
		FinishLatentTask(OwnerComp,EBTNodeResult::Failed);
		return;
	}
	// Get the current target location from the blackboard
	FVector _currentTargetLocation = _bbComponent->GetValueAsVector("TargetLocation");

	// Check if the target location has changed
	if(_currentTargetLocation != _previousTargetLocation)
	{
		//Move ai to the updated location
		_aiEnemyController->MoveToLocation(_currentTargetLocation);
		_previousTargetLocation = _currentTargetLocation;
	}
	
	//check if ai has reached the location
	FVector _aiActorLocation = _aiPawn->GetActorLocation();
	float _distanceToPlayer = FVector::Dist(_currentTargetLocation, _aiActorLocation);

	UE_LOG(LogTemp, Warning, TEXT("Distance to player: %f"), _distanceToPlayer);
	float _attackDistance = 80.0f;
	if(_distanceToPlayer <= _attackDistance)
	{
		
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	// else
	// {
	// 	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	// }
	
	

}
