// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/WeaponBase.h"
#include "Shotgun.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTM_API AShotgun : public AWeaponBase
{
	GENERATED_BODY()

	AShotgun();
	
	virtual void Fire(APlayerBase* _player, UWorld* _world) override;

	virtual void Reload() override;
	
};
