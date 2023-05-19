// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"



AEnemyAIController::AEnemyAIController()
{
	_behaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behavior Tree Component"));
	_blackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard Component"));



}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Encontrei BT"));
	if (IsValid(_behaviorTree.Get()))
	{
		RunBehaviorTree(_behaviorTree.Get());
		_behaviorTreeComponent->StartTree(*_behaviorTree.Get());

		UE_LOG(LogTemp, Warning, TEXT("Encontrei BT"));
	}
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if(IsValid(Blackboard.Get()) && IsValid(_behaviorTree.Get()))
	{
		Blackboard->InitializeBlackboard(*_behaviorTree.Get()->BlackboardAsset.Get());
	}

}
