// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyBase.h"

#include "PlayerBase.h"
#include "Components/CapsuleComponent.h"
#include "CharacterController.h"
#include "../ProjectMGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"


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

	
	
	
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
	AProjectMGameModeBase* _gameMode = Cast<AProjectMGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	if(_gameMode != nullptr)
	{
		bIsDead = false;
		_gameMode->_enemyAlive++;
		_gameMode->UpdateEnemiesAlive(_gameMode->_enemyAlive);
	
	}
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
	if(bIsAttacking!=true)
	{
		PerformSphereTrace();
		bIsAttacking = false;
	}
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
		UE_LOG(LogTemp, Warning, TEXT("sup yall its me its ded, hello ded im dad"));
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

		
		AProjectMGameModeBase* _gameMode = Cast<AProjectMGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

		if(_gameMode != nullptr)
		{
			if(bIsDead == false)
			_gameMode->_enemyAlive--;

			bIsDead = true;
			_gameMode->CheckEnemyAlive();
			GetWorld()->GetTimerManager().SetTimer(DestroyActorTimerHandle, this, &AEnemyBase::DestroyActor, 25, false);
		
			
		}

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
	FVector EndLocation = GetActorLocation();
	float Radius = 75.f;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);  // Ignore the current character
	TArray<FHitResult> HitResults;

//	if(HasAuthority())
//	{
		
		// Perform the Sphere Trace
		bool bHit = GetWorld()->SweepMultiByChannel(HitResults, StartLocation, EndLocation,FQuat::Identity, ECC_Pawn,FCollisionShape::MakeSphere(Radius),Params);
		
		if (bHit)
		{
			// Handle the hit result
			OnSphereTraceComplete(HitResults,Radius);
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
	for(const FHitResult& HitResult : HitResults)
	{
		AActor* HitActor = HitResult.GetActor();
		UE_LOG(LogTemp, Warning, TEXT("actor Hit: %s "), *HitActor->GetName());


		if(HitActor && HitActor->IsA(APlayerBase::StaticClass()))
		{
		
			APlayerBase* _hitPlayer = Cast<APlayerBase>(HitActor);
			if(_hitPlayer)
			{
				_hitPlayer->TakeDamage((_damage), FDamageEvent(), nullptr, this);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("nao bati num player i guess"));
		
			}
		}
		
		// Draw debug sphere
		FColor SphereColor = FColor::Red;
		//DrawDebugSphere(GetWorld(), HitResult.Location, radius, 16, SphereColor, false, 2.f);
	
	}


}



void AEnemyBase::ApplyKnockback(FVector KnockbackDirection, float KnockbackStrength)
{
	// Normalize the knockback direction and apply the knockback strength
	FVector KnockbackForce = KnockbackDirection.GetSafeNormal() * KnockbackStrength;

	// Apply the knockback force to the character's movement component
	UCharacterMovementComponent* _characterMovement = GetCharacterMovement();
	if (_characterMovement)
	{
		UE_LOG(LogTemp,Warning, TEXT("character movement : %p"), _characterMovement)
		_characterMovement->AddImpulse(KnockbackForce, true);
	}
}

bool AEnemyBase::MultiApplyKnockback_Validate(FVector KnockbackDirection, float KnockbackStrength)
{
	return true;
}

void AEnemyBase::MultiApplyKnockback_Implementation(FVector KnockbackDirection, float KnockbackStrength)
{
	// Call the original ApplyKnockback function to apply the effect on the server
	ApplyKnockback(KnockbackDirection, KnockbackStrength);
}

void AEnemyBase::DestroyActor()
{
	GetWorld()->GetTimerManager().ClearTimer(DestroyActorTimerHandle);
	Destroy();
}


