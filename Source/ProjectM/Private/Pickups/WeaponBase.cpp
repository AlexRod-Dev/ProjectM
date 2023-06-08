// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/WeaponBase.h"


AWeaponBase::AWeaponBase(UStaticMesh* InBaseMesh)
{

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;

	if(InBaseMesh)
	{
		WeaponMesh->SetStaticMesh(InBaseMesh);
		WeaponMesh->SetCollisionProfileName(TEXT("NoCollision"));
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

void AWeaponBase::Fire_Implementation()
{
}


