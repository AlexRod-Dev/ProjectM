// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyBase.h"

#include "PlayerBase.h"
#include "Components/CapsuleComponent.h"


// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_maxHealth = 100.0f;

	_currentHealth = _maxHealth;

	_damage = 30.0f;
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AEnemyBase::TakeDamage(float _damageTaken, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* _otherActor)
{
	float _damageApplied = _currentHealth - _damageTaken;
	SetCurrentHealth(_damageApplied);

	
	
	return _damageApplied;
}

void AEnemyBase::AttackPlayer()
{
	PerformSphereTrace();
}




void AEnemyBase::SetCurrentHealth(float _hpValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		_currentHealth = FMath::Clamp(_hpValue, 0.f, _maxHealth);
		OnHealthUpdate();
	}
}

void AEnemyBase::OnHealthUpdate()
{

	//All machines
	if (_currentHealth <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("sup yall its me ded"));
		Die();

	}

}

void AEnemyBase::OnRep_CurrentHealth()
{
	OnHealthUpdate();
}

void AEnemyBase::Die()
{

	if (GetLocalRole() == ROLE_Authority)
	{
		MultiDie();
	
	}


}

void AEnemyBase::MultiDie_Implementation()
{
	//Ragdoll
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);


}

void AEnemyBase::PerformSphereTrace()
{
	FVector StartLocation = GetActorLocation();
	FVector EndLocation = GetActorForwardVector() * 250.0f; 
	float Radius = 100.f;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);  // Ignore the current character
	TArray<FHitResult> HitResults;

	// Perform the Sphere Trace
	bool bHit = GetWorld()->SweepMultiByChannel(HitResults, StartLocation,
		EndLocation,
		FQuat::Identity,
		ECC_Pawn,  // Example collision channel
		FCollisionShape::MakeSphere(Radius),
		Params
	);

	if (bHit)
	{
		
		// Handle the hit result
		OnSphereTraceComplete(HitResults,Radius);
	}
	
}

void AEnemyBase::OnSphereTraceComplete(const TArray<FHitResult>& HitResults, float radius)
{
	// Handle the hit result here
	// You can access information such as HitResult.Actor, HitResult.Location, etc.
	
	for(const FHitResult& HitResult : HitResults)
	{
		AActor* HitActor = HitResult.GetActor();
		UE_LOG(LogTemp, Warning, TEXT("actor Hit: %s "), *HitActor->GetName());


		if(HitActor && HitActor->IsA(APlayerBase::StaticClass()))
		{
		
			APlayerBase* HitCharacter = Cast<APlayerBase>(HitActor);
			if(HitCharacter)
			{
				HitCharacter->TakeDamage((_damage), FDamageEvent(), nullptr, this);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("nao bati num player i guess"));
		
			}
		}
		// Example: Draw a debug sphere at the hit location
	
		FColor SphereColor = FColor::Red;
	
		DrawDebugSphere(GetWorld(), HitResult.Location, radius, 16, SphereColor, false, 5.f);
	
	}


}



