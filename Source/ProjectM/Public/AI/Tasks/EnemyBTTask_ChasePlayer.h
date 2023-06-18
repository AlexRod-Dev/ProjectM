// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "EnemyBTTask_ChasePlayer.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTM_API UEnemyBTTask_ChasePlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UEnemyBTTask_ChasePlayer();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	FVector _previousTargetLocation;
};
