// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "EnemyBTTask_AttackPlayer.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTM_API UEnemyBTTask_AttackPlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UEnemyBTTask_AttackPlayer();


	
private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	

};
