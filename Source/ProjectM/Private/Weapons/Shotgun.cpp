// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Shotgun.h"

#include "Player/BulletBase.h"
#include "Player/PlayerBase.h"

AShotgun::AShotgun() : AWeaponBase(
	LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Assets/Weapons/Mesh_Shotgun.Mesh_Shotgun")))
{
	_damage = 40.0f;
	_fireRate = 1.f;
	_totalAmmo = 40;
	_magSize = 32;
	_currentAmmo = _magSize;
	_reloadTime = 2.0f;

	static ConstructorHelpers::FClassFinder<ABulletBase> AmmoClassFinder(
		TEXT("/Game/Blueprints/Objects/Weapons/BP_Bullet_Shotgun"));
	if (AmmoClassFinder.Succeeded())
	{
		BulletClass = AmmoClassFinder.Class;
	}
}

void AShotgun::BeginPlay()
{
	Super::BeginPlay();
}

void AShotgun::ServerFire(APlayerBase* _player, UWorld* _world, float _timeSinceLastShot)
{
	Super::ServerFire(_player, _world, _timeSinceLastShot);

	UE_LOG(LogTemp, Warning, TEXT("Shoot Shotgun"));

	if (_currentAmmo > 0)
	{
		if (_timeSinceLastShot > _fireRate)
		{
			if (_world)
			{
				const int32 _numPellets = 8; // Number of shotgun pellets
				const float _spreadAngle = 20.0f; // Spread angle of the shotgun pellets

				const FVector _shootDirection = _player->GetActorForwardVector(); // Get the direction to shoot

				FVector _spawnLocation = _player->GetActorLocation() + (_player->GetActorForwardVector() * 54.0f);
				FRotator _spawnRotation = _player->GetActorRotation();

				// Loop over the number of shotgun pellets
				for (int32 PelletIndex = 0; PelletIndex < _numPellets; ++PelletIndex)
				{
					// Calculate the spread direction based on the spread angle
					const FVector _spreadDirection = FMath::VRandCone(_shootDirection,
					                                                  FMath::DegreesToRadians(_spreadAngle));

					// Spawn a shotgun bullet at the weapon's muzzle location and rotation
					FActorSpawnParameters _spawnParams;
					_spawnParams.Owner = _player;
					_spawnParams.Instigator = _player->GetInstigator();

					ABulletBase* ShotgunBullet = _world->SpawnActor<ABulletBase>(
						BulletClass, _spawnLocation, _spawnRotation, _spawnParams);

					_currentAmmo--;

					if (_currentAmmo < 0)
					{
						_currentAmmo = 0;
					}

					if (ShotgunBullet)
					{
						// Set the bullet spread
						ShotgunBullet->SetInitialVelocity(_spreadDirection * 1000.f);
						//reset timer
						Cast<ACharacterController>(_player->GetController())->_timeSinceLastShot = 0;
					}
				}
			}
		}
	}
	else
	{
		Reload();
		//Play sound
	}
}

void AShotgun::Reload()
{
	Super::Reload();

	if (_totalAmmo == 0)
	{
		return;
	}

	if (_totalAmmo < _magSize)
	{
		_currentAmmo = _totalAmmo;
		_totalAmmo = 0;
	}
	else
	{
		int32 _remainingBullets = _magSize - _currentAmmo;
		_currentAmmo += _remainingBullets;
		_totalAmmo -= _remainingBullets;
	}
}

void AShotgun::AddAmmo()
{
	Super::AddAmmo();

	if (HasAuthority())
	{
		_totalAmmo += _magSize;
	}
}

void AShotgun::ServerAddAmmo()
{
	Super::ServerAddAmmo();

	AddAmmo();
}
