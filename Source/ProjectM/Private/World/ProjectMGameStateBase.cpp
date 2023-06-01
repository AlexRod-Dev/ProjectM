// Fill out your copyright notice in the Description page of Project Settings.


#include "World/ProjectMGameStateBase.h"



AProjectMGameStateBase::AProjectMGameStateBase()
{
    SetReplicates(true);
    
    _enemiesAlive = 0;
    _currentWave = 1;
}

void AProjectMGameStateBase::UpdateEnemiesAlive(int32 _enemies)
{
    if (HasAuthority())
    {
        _enemiesAlive += _enemies;
    }
}

void AProjectMGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AProjectMGameStateBase, _enemiesAlive);
    DOREPLIFETIME(AProjectMGameStateBase, _currentWave);
}

