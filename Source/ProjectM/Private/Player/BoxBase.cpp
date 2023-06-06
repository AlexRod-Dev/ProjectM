// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BoxBase.h"

#include "Net/UnrealNetwork.h"

// Sets default values
ABoxBase::ABoxBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;

	_health = 100.f;

	// Get a reference to the material you want to apply to the mesh
	_redMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/LevelPrototyping/Materials/M_Solid_Red.M_Solid_Red"));
	_yellowMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/LevelPrototyping/Materials/M_Solid_Yellow.M_Solid_Yellow"));

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

	
	if(_health < 70.f && _health > 40.f)
	{
		_currentColor = _yellowMaterial;
		ChangeBoxColor(this,_currentColor);
		
	}

	if(_health <=40.f && _health > 0.1f)
	{
		_currentColor = _redMaterial;
		ChangeBoxColor(this,_currentColor);
		
	}

	if(_health <= 0.0f)
	{
		ServerDestroyBox();
	}

}

bool ABoxBase::ServerChangeBoxColor_Validate(UMaterialInterface* _material)
{
	return true;
}

void ABoxBase::ChangeBoxColor(ABoxBase* _box, UMaterialInterface* _material)
{
	if(HasAuthority())
	{
		_box->ServerChangeBoxColor(_material);
	}
}


void ABoxBase::ServerChangeBoxColor_Implementation(UMaterialInterface* _material)
{
	if(HasAuthority())
	StaticMesh->SetMaterial(0, _material);
	
}



void ABoxBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABoxBase, _health);
	DOREPLIFETIME(ABoxBase, _currentColor);
}

void ABoxBase::OnRep_Material()
{
	StaticMesh->SetMaterial(0,_currentColor);
}

bool ABoxBase::ServerTakeDamage_Validate(float _damage)
{
	return true;
}

