// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponBase.h"
#include "Rifle.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTM_API ARifle : public AWeaponBase
{
	GENERATED_BODY()

public:
	ARifle();
	virtual void BeginPlay() override;
	virtual void ServerFire(APlayerBase* _player, UWorld* _world, float _timeSinceLastShot) override;

	virtual void Reload() override;
};
