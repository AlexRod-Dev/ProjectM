// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/EnemyBTTask_FindClosestPlayer.h"

#include "AIController.h"
#include "EnemyAIController.h"
#include "EngineUtils.h"
#include "NavigationSystem.h"
#include "PlayerBase.h"
#include "BehaviorTree/BlackboardComponent.h"

UEnemyBTTask_FindClosestPlayer::UEnemyBTTask_FindClosestPlayer()
{
	NodeName = TEXT("Find Closest Player");
	
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UEnemyBTTask_FindClosestPlayer, BlackboardKey));

	bNotifyTick = true;
}

EBTNodeResult::Type UEnemyBTTask_FindClosestPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
		AEnemyAIController* _aiEnemyController {(Cast<AEnemyAIController>(OwnerComp.GetAIOwner()))};
		//Signal the behavior tree to call ticktask every frame
		OwnerComp.GetAIOwner()->SetFocus(_aiEnemyController->GetPawn());
	
		return EBTNodeResult::InProgress;

}

void UEnemyBTTask_FindClosestPlayer::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	
	
	//Get AI Pawn            {  } is a modern way to initialize aka initializer list
	AEnemyAIController* _aiEnemyController {(Cast<AEnemyAIController>(OwnerComp.GetAIOwner()))};
	const APawn* _aiPawn {_aiEnemyController->GetPawn()};

	APawn* _closestPlayer{nullptr};
	float _closestDistance{TNumericLimits<float>::Max()};

	//Iterate over all pawns in the world
	for(TActorIterator<APlayerBase> It(GetWorld()); It; ++It)
	{
		APawn* _pawn = *It;
		//Ignore own pawn
		if(_pawn != _aiPawn)
		{
			
			float _distance = FVector::Distance(_aiPawn->GetActorLocation(), _pawn->GetActorLocation());
			//If this pawn is closer update the variable
			if(_distance<_closestDistance)
			{
				_closestPlayer = _pawn;
				_closestDistance = _distance;
			}
		}
	}
		_aiEnemyController->GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, _closestPlayer->GetActorLocation());

	// Continue the task
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);


}


void UEnemyBTTask_FindClosestPlayer::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

	OwnerComp.GetAIOwner()->ClearFocus(EAIFocusPriority::Gameplay);
	
}


FString UEnemyBTTask_FindClosestPlayer::GetStaticDescription() const
{
	return FString::Printf(TEXT("vector: %s"), *BlackboardKey.SelectedKeyName.ToString());
}


