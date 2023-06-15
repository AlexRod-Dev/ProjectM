// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/WeaponBase.h"




AWeaponBase::AWeaponBase(USkeletalMesh* InBaseMesh)
{

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;

	if(InBaseMesh)
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


void AWeaponBase::Fire_Implementation(APlayerBase* _player, UWorld* _world)
{
}

void AWeaponBase::Reload_Implementation()
{
}

void AWeaponBase::GetLifetimeReplicatedProps(TArray <FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeaponBase, _currentAmmo);
	DOREPLIFETIME(AWeaponBase, _ammoCapacity);
}


