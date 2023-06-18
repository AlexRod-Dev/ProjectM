// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponBase.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"


AWeaponBase::AWeaponBase(USkeletalMesh* InBaseMesh)
{
	PrimaryActorTick.bCanEverTick = true;
	
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
	AudioComponent->SetupAttachment(RootComponent);

	if (InBaseMesh)
	{
		WeaponMesh->SetSkeletalMesh(InBaseMesh);
		WeaponMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	}
}

AWeaponBase::AWeaponBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// ...
}


void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

int32 AWeaponBase::GetCurrentAmmo()
{
	return _currentAmmo;
}

int32 AWeaponBase::GetMaxAmmo()
{
	return _ammoCapacity;
}

void AWeaponBase::Fire_Implementation(APlayerBase* _player, UWorld* _world, float _timeSinceLastShot)
{
}

void AWeaponBase::Reload_Implementation()
{
}

void AWeaponBase::Server_PlaySound_Implementation(USoundCue* _sound, FVector _location, UWorld* _world)
{
	// Call PlaySoundAtLocation on the server
	UGameplayStatics::PlaySoundAtLocation(_world, _sound, _location);

	// Replicate the sound to all clients
	Multicast_PlaySound( _sound, _location,_world);

}


void AWeaponBase::Multicast_PlaySound_Implementation(USoundCue* _sound, FVector _location, UWorld* _world)
{
	// Call PlaySoundAtLocation on the server
	UGameplayStatics::PlaySoundAtLocation(_world, _sound, _location);

}

bool AWeaponBase::Server_PlaySound_Validate(USoundCue* _sound, FVector _location, UWorld* _world)
{
	return true;
}

void AWeaponBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);


	DOREPLIFETIME(AWeaponBase, _currentAmmo);
	DOREPLIFETIME(AWeaponBase, _ammoCapacity);
	DOREPLIFETIME(AWeaponBase, AudioComponent);

	
}
