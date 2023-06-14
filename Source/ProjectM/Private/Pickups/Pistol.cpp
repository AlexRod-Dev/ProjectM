// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/Pistol.h"

#include "Player/BulletBase.h"
#include "Player/CharacterController.h"
#include "UObject/ConstructorHelpers.h"
#include "Player/PlayerBase.h"

APistol::APistol() : AWeaponBase(LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Assets/Weapons/Mesh_Pistol.Mesh_Pistol")))
{
	_damage = 25.0f;
	_fireRate = 0.5f;
	_ammoCapacity = 12;
	_currentAmmo = _ammoCapacity;
	_reloadTime = 1.0f;
	
	static ConstructorHelpers::FClassFinder<ABulletBase> AmmoClassFinder(TEXT("/Game/Blueprints/Objects/Weapons/BP_Bullet_Pistol"));
	if (AmmoClassFinder.Succeeded())
	{
		BulletClass = AmmoClassFinder.Class;
	}

}

void APistol::Fire(APlayerBase* _player, UWorld* _world)
{
	Super::Fire(_player, _world);

	UE_LOG(LogTemp, Warning, TEXT("Shoot Pistol"));

	
	if(_world)
	{
		FVector _spawnLocation = _player->GetActorLocation() + (_player->GetActorForwardVector() * 50.0f);
		FRotator _spawnRotation = _player->GetActorRotation();
	
		FActorSpawnParameters _spawnParameters;
		_spawnParameters.Instigator = _player->GetInstigator();
		_spawnParameters.Owner = _player;

		ABulletBase* _spawnedBullet = _world->SpawnActor<ABulletBase>(BulletClass, _spawnLocation, _spawnRotation, _spawnParameters);

		
		if(_spawnedBullet)
		{
			_spawnedBullet->_instigatorController =	Cast<ACharacterController>(_player->GetController());
		}
	}

	
}

