// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/EnemyBTTask_FindClosestPlayer.h"

#include "AIController.h"
#include "EngineUtils.h"
#include "NavigationSystem.h"
#include "PlayerBase.h"
#include "BehaviorTree/BlackboardComponent.h"

UEnemyBTTask_FindClosestPlayer::UEnemyBTTask_FindClosestPlayer()
{
	NodeName = TEXT("Find Closest Player");
	
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UEnemyBTTask_FindClosestPlayer, BlackboardKey));
}

EBTNodeResult::Type UEnemyBTTask_FindClosestPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	

	//Get AI Pawn            {  } is a modern way to initialize aka initializer list
	AAIController* _aIController {OwnerComp.GetAIOwner()};
	const APawn* _aIPawn {_aIController->GetPawn()};

	APawn* _closestPlayer = nullptr;
	float _closestDistance = TNumericLimits<float>::Max();

	//Iterate over all pawns in the world
	for(TActorIterator<APlayerBase> It(GetWorld()); It; ++It)
	{
		APawn* _pawn = *It;
		//Ignore own pawn
		if(_pawn == _aIPawn)
		{
			
		}

		float _distance = FVector::Distance(_aIPawn->GetActorLocation(), _pawn->GetActorLocation());

		//If this pawn is closer update the variable
		if(_distance<_closestDistance)
		{
			_closestPlayer = _pawn;
			_closestDistance = _distance;
		}
		_aIController->GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, _closestPlayer->GetActorLocation());
		UE_LOG(LogTemp, Warning, TEXT("follow player : %s "), *_closestPlayer->GetName());
	}

	//Signal the behavior tree component that the task finish with success
	
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;

}

FString UEnemyBTTask_FindClosestPlayer::GetStaticDescription() const
{
	return FString::Printf(TEXT("vector: %s"), *BlackboardKey.SelectedKeyName.ToString());
}