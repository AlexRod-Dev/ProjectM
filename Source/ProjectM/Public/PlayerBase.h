// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "PlayerBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTM_API APlayerBase : public ACharacter
{
	GENERATED_BODY()
	
public:
	// Sets default values for this pawn's properties
	APlayerBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }


	/** Property replication */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
		/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;


protected:
	virtual void BeginPlay() override;

	//Max Walk Speed
	UPROPERTY(EditAnywhere)
	float _maxMoveSpeed;

	//Current Walk Speed
	UPROPERTY(EditAnywhere)
	float _moveSpeed;

	//Max starting Health
	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float _maxHealth;

	//Current Player Health
	UPROPERTY(BlueprintReadWrite, ReplicatedUsing=OnRep_CurrentHealth)
	float _currentHealth;

	UPROPERTY(BlueprintReadWrite, Category = "Health")
	bool bIsDead;

	/** RepNotify for changes made to current health.*/
	UFUNCTION()
	void OnRep_CurrentHealth();

	/** Response to health being updated. Called on the server immediately after modification, and on clients in response to a RepNotify*/
	void OnHealthUpdate();

	FTimerHandle RespawnTimerHandle;

public:
	//Getter for Max Health
	UFUNCTION(BlueprintPure, Category="Health")
	FORCEINLINE float GetMaxHealth() const { return _maxHealth; }

	//Getter for Current Health
	UFUNCTION(BlueprintPure, Category = "Health")
	FORCEINLINE float GetCurrentHealth() const { return _currentHealth; }

	//Setter for Current Health
	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetCurrentHealth(float _hpValue);

	//Function for Taking Damage
	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual float TakeDamage(float _damageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* _otherActor) override;

	//Function for Recover Health
	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual float RecoverHealth(float _amount);
	
	void Die();

	UFUNCTION(NetMulticast, Reliable)
	void MultiDie();
	void MultiDie_Implementation();


	void Respawn();

	void HandleRespawnTimer();
};
