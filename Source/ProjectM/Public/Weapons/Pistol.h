// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/PlayerBase.h"
#include "Weapons/WeaponBase.h"
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
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	
	virtual void ServerFire(APlayerBase* _player, UWorld* _world, float _timeSinceLastShot) override;

	virtual void MultiReload() override;
	// Set the bullet class
	virtual void Multicast_PlaySound(USoundCue* _sound, FVector _location, UWorld* _world) override;

	virtual void Client_PlaySound(USoundCue* _sound, FVector _location, UWorld* _world) override;
	virtual void AddAmmo() override;
};
