// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Rifle.h"

#include "Player/BulletBase.h"
#include "Player/CharacterController.h"
#include "Player/PlayerBase.h"

ARifle::ARifle() : AWeaponBase(LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Assets/Weapons/Mesh_Rifle.Mesh_Rifle")))
{
	_damage = 40.0f;
	_fireRate = 0.1f;
	_totalAmmo = 60;
	_magSize = 30;
	_currentAmmo = _magSize;
	_reloadTime = 2.0f;

	static ConstructorHelpers::FClassFinder<ABulletBase> AmmoClassFinder(
		TEXT("/Game/Blueprints/Objects/Weapons/BP_Bullet_Rifle"));
	if (AmmoClassFinder.Succeeded())
	{
		BulletClass = AmmoClassFinder.Class;
	}
}

void ARifle::BeginPlay()
{
	Super::BeginPlay();
}

void ARifle::ServerFire(APlayerBase* _player, UWorld* _world, float _timeSinceLastShot)
{
	Super::ServerFire(_player, _world, _timeSinceLastShot);


	if (_currentAmmo > 0)
	{
		if (_timeSinceLastShot > _fireRate)
		{
			if (_world)
			{
				FVector _spawnLocation = _player->GetActorLocation() + (_player->GetActorForwardVector() * 54.0f);
				FRotator _spawnRotation = _player->GetActorRotation();

				FActorSpawnParameters _spawnParameters;
				_spawnParameters.Instigator = _player->GetInstigator();
				_spawnParameters.Owner = _player;

				ABulletBase* _spawnedBullet = _world->SpawnActor<ABulletBase>(
					BulletClass, _spawnLocation, _spawnRotation, _spawnParameters);

				_currentAmmo--;
				if (_currentAmmo < 0)
				{
					_currentAmmo = 0;
				}

				if (_spawnedBullet)
				{
					_spawnedBullet->_instigatorController = Cast<ACharacterController>(_player->GetController());
					_spawnedBullet->SetInitialVelocity(_player->GetActorForwardVector() * 2000.f);
					//reset timer
					Cast<ACharacterController>(_player->GetController())->_timeSinceLastShot = 0;
				}
			}
		}
	}
	else
	{
		Cast<ACharacterController>(_player->GetController())->StartReload();
		//Reload();
		//Play empty magazine sound
	}
}

void ARifle::MultiReload()
{
	Super::MultiReload();

	if (_totalAmmo == 0)
	{
		return;
	}

	if (_totalAmmo <= _magSize)
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

void ARifle::AddAmmo()
{
	Super::AddAmmo();

	if (HasAuthority())
	{
		_totalAmmo += _magSize;
	}
}

void ARifle::ServerAddAmmo()
{
	Super::ServerAddAmmo();

	AddAmmo();
}
