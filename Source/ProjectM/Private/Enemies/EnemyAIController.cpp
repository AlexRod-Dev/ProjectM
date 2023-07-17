// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"


AEnemyAIController::AEnemyAIController()
{
	_behaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behavior Tree Component"));
	_blackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard Component"));
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	
	if (IsValid(_behaviorTree.Get()))
	{
		RunBehaviorTree(_behaviorTree.Get());
		_behaviorTreeComponent->StartTree(*_behaviorTree.Get());
	
	}
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (IsValid(Blackboard.Get()) && IsValid(_behaviorTree.Get()))
	{
		Blackboard->InitializeBlackboard(*_behaviorTree.Get()->BlackboardAsset.Get());
	}
}

void AEnemyAIController::ApplyKnockback(float _knockbackStrength, FVector _knockbackDirection)
{
	if(_behaviorTreeComponent != nullptr)
	{
		_behaviorTreeComponent->StopTree(EBTStopMode::Forced);
	}
	if(HasAuthority())
	{
		
	APawn* _controlledPawn = GetPawn();
	if (_controlledPawn)
	{
		ACharacter* _controlledCharacter = Cast<ACharacter>(_controlledPawn);
		if (_controlledCharacter)
		{
			FVector _launchVelocity = _knockbackDirection * _knockbackStrength;
			_controlledCharacter->LaunchCharacter(_launchVelocity, true, true);

			if (GetNetMode() == NM_Client)
			{
				ServerApplyKnockback(_knockbackStrength, _knockbackDirection);
			}
		}
	}
	}
	// if(_behaviorTreeComponent != nullptr)
	// {
	// 	_behaviorTreeComponent->StartTree(*_behaviorTree.Get());
	// }
}



void AEnemyAIController::ServerApplyKnockback_Implementation(float _knockbackStrength, FVector _knockbackDirection)
{
	ApplyKnockback(_knockbackStrength, _knockbackDirection);
}
