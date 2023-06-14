// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/Shotgun.h"

#include "Player/BulletBase.h"
#include "Player/PlayerBase.h"

AShotgun::AShotgun() : AWeaponBase(LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Assets/Weapons/Mesh_Shotgun.Mesh_Shotgun")))
{
	_damage = 40.0f;
	_fireRate = 0.1f;
	_ammoCapacity = 30;
	_reloadTime = 2.0f;

	static ConstructorHelpers::FClassFinder<ABulletBase> AmmoClassFinder(TEXT("/Game/Blueprints/Objects/Weapons/BP_Bullet_Shotgun"));
	if (AmmoClassFinder.Succeeded())
	{
		BulletClass = AmmoClassFinder.Class;
	}
	
}

void AShotgun::Fire(APlayerBase* _player, UWorld* _world)
{
	Super::Fire(_player, _world);

	UE_LOG(LogTemp, Warning, TEXT("Shoot Shotgun"));

	if (_world)
	{
		const int32 _numPellets = 8; // Number of shotgun pellets
		const float _spreadAngle = 20.0f; // Spread angle of the shotgun pellets

		const FVector _shootDirection = _player->GetActorForwardVector(); // Get the direction to shoot

		FVector _spawnLocation = _player->GetActorLocation() + (_player->GetActorForwardVector() * 50.0f);
		FRotator _spawnRotation = _player->GetActorRotation();
	
		// Loop over the number of shotgun pellets
		for (int32 PelletIndex = 0; PelletIndex < _numPellets; ++PelletIndex)
		{
			// Calculate the spread direction based on the spread angle
			const FVector _spreadDirection = FMath::VRandCone(_shootDirection, FMath::DegreesToRadians(_spreadAngle));
			
			// Spawn a shotgun bullet at the weapon's muzzle location and rotation
			FActorSpawnParameters _spawnParams;
			_spawnParams.Owner = _player;
			_spawnParams.Instigator = _player->GetInstigator();
			
			ABulletBase* ShotgunBullet = _world->SpawnActor<ABulletBase>(BulletClass, _spawnLocation, _spawnRotation, _spawnParams);

			if (ShotgunBullet)
			{
				//ShotgunBullet->
				// Set properties for the shotgun bullet, such as initial velocity and damage
				ShotgunBullet->SetInitialVelocity(_spreadDirection * 1000.f);
				
			}
		}
	}


}