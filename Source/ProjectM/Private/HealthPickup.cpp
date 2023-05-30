// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPickup.h"

#include "PlayerBase.h"

// Sets default values
AHealthPickup::AHealthPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create a sphere collision component for the pickup
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->InitSphereRadius(50.0f);
	SphereComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	RootComponent = SphereComponent;


	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMesh->SetupAttachment(RootComponent);
	
	// Set default health amount
	_healthAmount = 25.0f;

	// Bind the overlap event
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AHealthPickup::OnComponentBeginOverlap);

}

// Called when the game starts or when spawned
void AHealthPickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHealthPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHealthPickup::Server_RecoverHp_Implementation(APlayerBase* _playerCharacter)
{
	if (_playerCharacter)
	{
		_playerCharacter->RecoverHealth(_healthAmount);
		Destroy();
	}
}

bool AHealthPickup::Server_RecoverHp_Validate(APlayerBase* _playerCharacter)
{
	return _playerCharacter != nullptr;
}

void AHealthPickup::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor != nullptr && OtherActor !=this)
	{
		APlayerBase* _playerCharacter = Cast<APlayerBase>(OtherActor);
		if(_playerCharacter)
		{
			if(_playerCharacter->GetCurrentHealth() > _playerCharacter->GetMaxHealth()-0.1f)
			{
				return;
			}
			Server_RecoverHp(_playerCharacter);
		}
			
		
	}
	
}



