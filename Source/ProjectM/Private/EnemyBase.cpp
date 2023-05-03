// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyBase.h"
#include "Components/CapsuleComponent.h"


// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_maxHealth = 100.0f;

	_currentHealth = _maxHealth;

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



