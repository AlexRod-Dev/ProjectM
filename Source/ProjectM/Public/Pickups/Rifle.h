// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/WeaponBase.h"
#include "Rifle.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTM_API ARifle : public AWeaponBase
{
	GENERATED_BODY()

	ARifle();
	
	virtual void Fire(APlayerBase* _player, UWorld* _world) override;

	virtual void Reload() override;
};
