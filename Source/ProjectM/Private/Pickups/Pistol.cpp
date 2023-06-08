// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/Pistol.h"

APistol::APistol() : AWeaponBase(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Assets/Weapons/Mesh_Pistol.Mesh_Pistol")))
{
	_damage = 25.0f;
	_fireRate = 0.5f;
	_ammoCapacity = 12;
	_reloadTime = 2.0f;
	
}

void APistol::Fire()
{
	Super::Fire();

	
}

