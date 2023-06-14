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


void AWeaponBase::Fire_Implementation(APlayerBase* _player, UWorld* _world)
{
}


