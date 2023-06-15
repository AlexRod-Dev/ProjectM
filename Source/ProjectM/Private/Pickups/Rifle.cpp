// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/Rifle.h"

#include "Player/BulletBase.h"
#include "Player/CharacterController.h"
#include "Player/PlayerBase.h"

ARifle::ARifle() : AWeaponBase(LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Assets/Weapons/Mesh_Rifle.Mesh_Rifle")))
{
	_damage = 40.0f;
	_fireRate = 0.1f;
	_ammoCapacity = 30;
	_currentAmmo = _ammoCapacity;
	_reloadTime = 2.0f;

	static ConstructorHelpers::FClassFinder<ABulletBase> AmmoClassFinder(TEXT("/Game/Blueprints/Objects/Weapons/BP_Bullet_Rifle"));
	if (AmmoClassFinder.Succeeded())
	{
		BulletClass = AmmoClassFinder.Class;
	}
}

void ARifle::Fire(APlayerBase* _player, UWorld* _world)
{
	Super::Fire(_player, _world);
	UE_LOG(LogTemp, Warning, TEXT("Shoot Rifle"));

	if(_currentAmmo>0)
	{
			
		
			if(_world)
			{
				FVector _spawnLocation = _player->GetActorLocation() + (_player->GetActorForwardVector() * 50.0f);
				FRotator _spawnRotation = _player->GetActorRotation();
			
				FActorSpawnParameters _spawnParameters;
				_spawnParameters.Instigator = _player->GetInstigator();
				_spawnParameters.Owner = _player;

				ABulletBase* _spawnedBullet = _world->SpawnActor<ABulletBase>(BulletClass, _spawnLocation, _spawnRotation, _spawnParameters);

				_currentAmmo--;
				if(_currentAmmo<0)
					_currentAmmo = 0;
				
				if(_spawnedBullet)
				{
					_spawnedBullet->_instigatorController =	Cast<ACharacterController>(_player->GetController());
					_spawnedBullet->SetInitialVelocity(_player->GetActorForwardVector() * 2000.f);
				}
			}
			
	}else
	{
		//Play empty magazine sound
	}

}

void ARifle::Reload()
{
	Super::Reload();

	_currentAmmo = _ammoCapacity;
}


