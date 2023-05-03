// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"

UCLASS()
class PROJECTM_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Event for Taking Damage
	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual float TakeDamage(float _damageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* _otherActor) override;

	//Getter for Max Health
	UFUNCTION(BlueprintPure, Category="Health")
	FORCEINLINE float GetMaxHealth() const { return _maxHealth; }

	//Getter for Current Health
	UFUNCTION(BlueprintPure, Category = "Health")
	FORCEINLINE float GetCurrentHealth() const { return _currentHealth; }

	//Setter for Current Health
	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetCurrentHealth(float _hpValue);


protected:
	float _maxHealth;
	float _currentHealth;

	/** RepNotify for changes made to current health.*/
	UFUNCTION()
	void OnRep_CurrentHealth();

	/** Response to health being updated. Called on the server immediately after modification, and on clients in response to a RepNotify*/
	void OnHealthUpdate();

	

	void Die();

	UFUNCTION(NetMulticast, Reliable)
	void MultiDie();
	void MultiDie_Implementation();



};
