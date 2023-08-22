// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Pistol.h"

#include "Kismet/GameplayStatics.h"
#include "Player/BulletBase.h"
#include "Player/CharacterController.h"
#include "UObject/ConstructorHelpers.h"
#include "Player/PlayerBase.h"
#include "Sound/SoundCue.h"

APistol::APistol() : AWeaponBase(
	LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Assets/Weapons/Mesh_Pistol")))
{
	PrimaryActorTick.bCanEverTick = true;


	_damage = 25.0f;
	_fireRate = 0.4f;
	_magSize = 12;
	_currentAmmo = _magSize;
	_totalAmmo = 12;
	_reloadTime = 0.5f;


	bReplicates = true;


	// Set the bullet class
	static ConstructorHelpers::FClassFinder<ABulletBase> AmmoClassFinder(
		TEXT("/Game/Blueprints/Objects/Weapons/BP_Bullet_Pistol"));
	if (AmmoClassFinder.Succeeded())
	{
		BulletClass = AmmoClassFinder.Class;
	}

	//Set the sounds
	
	static ConstructorHelpers::FObjectFinder<USoundCue> FireSoundAsset(
		TEXT("/Game/Sounds/SFX/Weapons/Pistol/Cues/Pistol_Fire_Cue"));
	if (FireSoundAsset.Succeeded())
	{
		//AudioComponent->SetSound(FireSoundAsset.Object);
		FireSound = FireSoundAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> ReloadSoundAsset(
		TEXT("/Game/Sounds/SFX/Weapons/Pistol/Cues/Pistol_Reload_Cue"));
	if (ReloadSoundAsset.Succeeded())
	{
		ReloadSound = ReloadSoundAsset.Object;
	}
}

void APistol::BeginPlay()
{
	Super::BeginPlay();
}

void APistol::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void APistol::ServerFire(APlayerBase* _player, UWorld* _world, float _timeSinceLastShot)
{
	Super::ServerFire(_player, _world, _timeSinceLastShot);


	if (_currentAmmo > 0)
	{
		if (_timeSinceLastShot > _fireRate)
		{
			if (_world)
			{
			
				FVector _spawnLocation = _player->GetActorLocation() + (_player->GetActorForwardVector() * 56.0f);
				FRotator _spawnRotation = _player->GetActorRotation();

				FActorSpawnParameters _spawnParameters;
				_spawnParameters.Instigator = _player->GetInstigator();
				_spawnParameters.Owner = _player;

				ABulletBase* _spawnedBullet = _world->SpawnActor<ABulletBase>(
					BulletClass, _spawnLocation, _spawnRotation, _spawnParameters);

				_currentAmmo--;
				if (_currentAmmo <= 0)
				{
					_currentAmmo = 0;
				}

				if (_spawnedBullet)
				{
					_spawnedBullet->_instigatorController = Cast<ACharacterController>(_player->GetController());

					//reset timer
					Cast<ACharacterController>(_player->GetController())->_timeSinceLastShot = 0;

					if (FireSound)
					{
						if (HasAuthority())
						{
							Server_PlaySound(FireSound, _spawnLocation, _world);
						}
					}
				}
			}
		}
	}

	else
	{
		Cast<ACharacterController>(_player->GetController())->StartReload();
		//Play empty magazine sound
	}
}


void APistol::MultiReload()
{
	Super::MultiReload();

	_currentAmmo = _magSize;

	if (ReloadSound != nullptr)
	{
		//	if(HasAuthority())
		//	Server_PlaySound(FireSound,_spawnLocation,_world);

		UGameplayStatics::PlaySoundAtLocation(this, ReloadSound, GetActorLocation());
	}
}

void APistol::Multicast_PlaySound(USoundCue* _sound, FVector _location, UWorld* _world)
{
	Super::Multicast_PlaySound(_sound, _location, _world);

	
}

void APistol::AddAmmo()
{
	Super::AddAmmo();

	//pistol has unlimited ammo
}
