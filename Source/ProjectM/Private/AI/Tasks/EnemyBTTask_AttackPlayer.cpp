// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/EnemyBTTask_AttackPlayer.h"

#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "Enemies/EnemyBase.h"
#include "EngineUtils.h"

UEnemyBTTask_AttackPlayer::UEnemyBTTask_AttackPlayer()
{
	NodeName = TEXT("Attack Player");

	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UEnemyBTTask_AttackPlayer, BlackboardKey));
}


EBTNodeResult::Type UEnemyBTTask_AttackPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* _aiPawn = OwnerComp.GetAIOwner()->GetPawn();

	AEnemyBase* EnemyPawn = Cast<AEnemyBase>(_aiPawn);
	if (EnemyPawn)
	{
		EnemyPawn->PerformSphereTrace();
	}


	//Signal the behavior tree component that the task finish with success

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
