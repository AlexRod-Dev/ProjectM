// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/WeaponPickup.h"
#include "Player/PlayerBase.h"


// Sets default values
AWeaponPickup::AWeaponPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	// Create the trigger volume
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->InitSphereRadius(50.0f);
	SphereComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	RootComponent = SphereComponent;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	StaticMesh->SetupAttachment(RootComponent);

	
	// Bind the OnOverlapBegin function to the OnComponentBeginOverlap event
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AWeaponPickup::OnComponentBeginOverlap);

	

}

// Called when the game starts or when spawned
void AWeaponPickup::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeaponPickup::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor != nullptr && OtherActor != this)
	{
		APlayerBase* _player = Cast<APlayerBase>(OtherActor);

		if(_player)
		{
			_player->PickupWeapon(this);
		}
	}
	Destroy();
	
}





