// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxBase.h"

#include "Net/UnrealNetwork.h"

// Sets default values
ABoxBase::ABoxBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;

	_health = 100.f;
}

// Called when the game starts or when spawned
void ABoxBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABoxBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABoxBase::ServerDestroyBox_Implementation()
{
	Destroy();
}

bool ABoxBase::ServerDestroyBox_Validate()
{
	return true;
}

void ABoxBase::ServerTakeDamage_Implementation(float _damage)
{
	_health -= _damage;
	if(_health <= 0.0f)
	{
		ServerDestroyBox();
	}
}

void ABoxBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABoxBase, _health);
}

bool ABoxBase::ServerTakeDamage_Validate(float _damage)
{
	return true;
}

