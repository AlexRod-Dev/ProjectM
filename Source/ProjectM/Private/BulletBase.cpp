// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletBase.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerBase.h"
#include "EnemyBase.h"
#include "UObject/ConstructorHelpers.h"


// Sets default values
ABulletBase::ABulletBase()
{

	bReplicates = true;

	_damageType = UDamageType::StaticClass();
	
	_damage = 20.0f;

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	SphereComponent->InitSphereRadius(2.0f);
	SphereComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	RootComponent = SphereComponent;

	if(GetLocalRole() == ROLE_Authority)
	{
		SphereComponent->OnComponentHit.AddDynamic(this, &ABulletBase::OnProjectileImpact);
	}

	//Definition for the Mesh that will serve as your visual representation.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultMesh(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMesh->SetupAttachment(RootComponent);

	//Set the Static Mesh and its position/scale if you successfully found a mesh asset to use.
	if (DefaultMesh.Succeeded())
	{
		StaticMesh->SetStaticMesh(DefaultMesh.Object);
		StaticMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -2.0f));
		StaticMesh->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> DefaultBloodEffect(TEXT("/Game/StarterContent/Particles/P_Blood_Splat_Cone.P_Blood_Splat_Cone"));
	if (DefaultBloodEffect.Succeeded())
	{
		_bloodEffect = DefaultBloodEffect.Object;
	}

	//Definition for the Projectile Movement Component.
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComponent->SetUpdatedComponent(SphereComponent);
	ProjectileMovementComponent->InitialSpeed = 1500.0f;
	ProjectileMovementComponent->MaxSpeed = 1500.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

}

// Called when the game starts or when spawned
void ABulletBase::BeginPlay()
{
	Super::BeginPlay();

	//The bullets destroys after 2.5 seconds.
	SetLifeSpan(2.5f);
}

void ABulletBase::Destroyed()
{
	
}

// Called every frame
void ABulletBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABulletBase::OnProjectileImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(OtherActor)
	{
		//for Teammates take half hp
		if(APlayerBase* player = Cast<APlayerBase>(OtherActor))
		{
			if(HasAuthority())
			player->TakeDamage((_damage / 2), FDamageEvent(), nullptr, this);

			FVector _spawnLocation = GetActorLocation();
			UGameplayStatics::SpawnEmitterAtLocation(this, _bloodEffect, _spawnLocation, FRotator::ZeroRotator, true, EPSCPoolMethod::AutoRelease);

		}

		if(AEnemyBase* enemy = Cast<AEnemyBase>(OtherActor))
		{
			if (HasAuthority())
			{
				
				enemy->TakeDamage(_damage, FDamageEvent(), _instigatorController, this);
			}

			FVector _knockbackDirection = -Hit.Normal;
			float _knockbackStrenght = 1000.f;
			enemy->ApplyKnockback(_knockbackStrenght,_knockbackDirection);
			// Call the MultiApplyKnockback function to apply the knockback effect
			
			FVector _spawnLocation = GetActorLocation();
			UGameplayStatics::SpawnEmitterAtLocation(this, _bloodEffect, _spawnLocation, FRotator::ZeroRotator, true, EPSCPoolMethod::AutoRelease);

		}
		
	}

	Destroy();

}



