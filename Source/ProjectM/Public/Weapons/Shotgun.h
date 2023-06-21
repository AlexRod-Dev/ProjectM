// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponBase.h"
#include "Shotgun.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTM_API AShotgun : public AWeaponBase
{
	GENERATED_BODY()

public:
	AShotgun();
	virtual void BeginPlay() override;
	virtual void ServerFire(APlayerBase* _player, UWorld* _world, float _timeSinceLastShot) override;

	virtual void Reload() override;
};
