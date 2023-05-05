// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "EnemyBTTask_FindClosestPlayer.generated.h"

/**
 * BTTask for finding the closest player in the map
 */
UCLASS()
class PROJECTM_API UEnemyBTTask_FindClosestPlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	

public:
	UEnemyBTTask_FindClosestPlayer();

private:
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;

protected:
	


	
};
