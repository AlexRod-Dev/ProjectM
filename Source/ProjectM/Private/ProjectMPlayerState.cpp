// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectMPlayerState.h"

#include "Net/UnrealNetwork.h"

AProjectMPlayerState::AProjectMPlayerState()
{

	
}

void AProjectMPlayerState::AddScore(int32 _points)
{
	if(HasAuthority())
	{
		_score += _points;
		OnRep_Score();
	}
}

int32 AProjectMPlayerState::GetScore() const
{
	return _score;
}

void AProjectMPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AProjectMPlayerState, _score);
}

void AProjectMPlayerState::OnRep_Score()
{
	Super::OnRep_Score();
}

