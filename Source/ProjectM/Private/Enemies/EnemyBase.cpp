// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemies/EnemyBase.h"

#include "Enemies/EnemyAIController.h"
#include "Pickups/HealthPickup.h"
#include "Player/PlayerBase.h"
#include "Components/CapsuleComponent.h"
#include "World/ProjectMGameStateBase.h"
#include "Player/ProjectMPlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/BoxBase.h"


// Sets default values
AEnemyBase::AEnemyBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_maxHealth = 100.0f;

	_currentHealth = _maxHealth;

	_damage = 25.0f;

	_attackSpeed = 1.0f;

	bIsAttacking = false;

	_damagedFrom = nullptr;


	// Set the health pickup class
	static ConstructorHelpers::FClassFinder<AHealthPickup> HealthPickupClassFinder(
		TEXT("/Game/Blueprints/BP_HealthPickup"));
	if (HealthPickupClassFinder.Succeeded())
	{
		HealthPickupClass = HealthPickupClassFinder.Class;
	}
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	bIsAlive = true;

	GetWorld()->GetGameState<AProjectMGameStateBase>()->UpdateEnemiesAlive(1);
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float AEnemyBase::TakeDamage(float _damageTaken, const FDamageEvent& DamageEvent, AController* _instigatorController,
                             AActor* _otherActor)
{
	AController* _controller = _otherActor->GetInstigatorController();

	if (_controller != nullptr)
	{
		_damagedFrom = Cast<ACharacterController>(_instigatorController);
	}

	float _damageApplied = _currentHealth - _damageTaken;

	if (bIsAlive)
	{
		SetCurrentHealth(_damageApplied);
	}

	return _damageApplied;
}

void AEnemyBase::AttackPlayer()
{
	if (bIsAttacking != true)
	{
		PerformSphereTrace();
		bIsAttacking = false;
	}
}


void AEnemyBase::SetCurrentHealth(float _hpValue)
{
	if (HasAuthority())
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
		Die();
	}
}

void AEnemyBase::OnRep_CurrentHealth()
{
	OnHealthUpdate();
}

void AEnemyBase::Die()
{
	bIsAlive = false;
	UWorld* _world = GetWorld();
	if (_world)
	{
		int32 _waveNumber = Cast<AProjectMGameStateBase>(_world->GetGameState())->GetCurrentWave();

		if (_damagedFrom != nullptr)
		{
			_damagedFrom->GetPlayerState<AProjectMPlayerState>()->AddScore(1 + _waveNumber);
		}
	}


	if (HasAuthority())
	{
		_world->GetGameState<AProjectMGameStateBase>()->UpdateEnemiesAlive(-1);
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

	// Spawn health pickup actor
	if (HealthPickupClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		FVector SpawnLocation = GetActorLocation();
		float ZOffset = 50.0f;
		SpawnLocation.Z -= ZOffset;
		FRotator SpawnRotation = GetActorRotation();


		AHealthPickup* HealthPickup = GetWorld()->SpawnActor<AHealthPickup>(
			HealthPickupClass, SpawnLocation, SpawnRotation, SpawnParams);
	}
}


void AEnemyBase::PerformSphereTrace()
{
	FVector StartLocation = GetActorLocation();
	FVector EndLocation = GetActorLocation();
	float Radius = 75.f;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this); // Ignore the current character
	TArray<FHitResult> HitResults;

	//	if(HasAuthority())
	//	{

	// Perform the Sphere Trace
	bool bHit = GetWorld()->SweepMultiByChannel(HitResults, StartLocation, EndLocation, FQuat::Identity, ECC_Pawn,
	                                            FCollisionShape::MakeSphere(Radius), Params);

	if (bHit)
	{
		// Handle the hit result
		OnSphereTraceComplete(HitResults, Radius);
	}


	//	}
	bIsAttacking = false;
}

void AEnemyBase::StartAttackTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);

	float _attackInterval = 1.0f / _attackSpeed;


	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &AEnemyBase::AttackPlayer, _attackInterval, true);
}

void AEnemyBase::OnSphereTraceComplete(const TArray<FHitResult>& HitResults, float radius)
{
	// Handle the hit result here
	for (const FHitResult& HitResult : HitResults)
	{
		AActor* HitActor = HitResult.GetActor();

		if (HitActor && HitActor->IsA(APlayerBase::StaticClass()))
		{
			APlayerBase* _hitPlayer = Cast<APlayerBase>(HitActor);
			if (_hitPlayer)
			{
				_hitPlayer->TakeDamage(_damage, FDamageEvent(), nullptr, this);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("nao bati num player i guess"));
			}
		}
		if (HitActor && HitActor->IsA(ABoxBase::StaticClass()))
		{
			ABoxBase* _hitBox = Cast<ABoxBase>(HitActor);
			if (_hitBox)
			{
				_hitBox->ServerTakeDamage(_damage);
			}
		}


		// Draw debug sphere
		FColor SphereColor = FColor::Red;
		//DrawDebugSphere(GetWorld(), HitResult.Location, radius, 16, SphereColor, false, 2.f);
	}
}

void AEnemyBase::ApplyKnockback(float _knockbackStrength, FVector _knockbackDirection)
{
	AController* _controller = GetController();
	if (_controller)
	{
		AEnemyAIController* _aiController = Cast<AEnemyAIController>(_controller);

		if (_aiController)
		{
			_aiController->ApplyKnockback(_knockbackStrength, _knockbackDirection);

			if (GetNetMode() == NM_Client)
			{
				ServerApplyKnockback(_knockbackStrength, _knockbackDirection);
			}
		}
	}
}

void AEnemyBase::ServerApplyKnockback_Implementation(float _knockbackStrength, FVector _knockbackDirection)
{
	ApplyKnockback(_knockbackStrength, _knockbackDirection);
}

bool AEnemyBase::ServerApplyKnockback_Validate(float KnockbackStrength, FVector KnockbackDirection)
{
	return true;
}
