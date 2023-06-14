// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/WeaponBase.h"
#include "Pistol.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTM_API APistol : public AWeaponBase
{
	GENERATED_BODY()

public:
	APistol();
	
	virtual void Fire(APlayerBase* _player, UWorld* _world) override;
};
