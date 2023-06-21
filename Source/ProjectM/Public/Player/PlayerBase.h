// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Weapons/WeaponBase.h"
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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

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

#pragma region  Health
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
#pragma endregion

	
public:
	
#pragma region  Health
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
	virtual float TakeDamage(float _damageTaken, const struct FDamageEvent& DamageEvent, AController* EventInstigator,
	                         AActor* _otherActor) override;

	//Function for Recover Health
	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual float RecoverHealth(float _amount);

	void Die();

	UFUNCTION(NetMulticast, Reliable)
	void MultiDie();
	void MultiDie_Implementation();


	void Respawn();

	void HandleRespawnTimer();

#pragma endregion 
	
#pragma region  Inventory
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category ="Weapon")
	TArray<TSubclassOf<AWeaponBase>> _weaponInventory;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Weapon")
	int32 _currentWeapIndex;

	int32 GetCurrentWeapIndex();

	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon, VisibleAnywhere, BlueprintReadOnly, Category ="Weapon")
	TSubclassOf<AWeaponBase> _equippedWeapon;

	TSubclassOf<AWeaponBase> GetEquippedWeapon();
	UFUNCTION()
	void OnRep_EquippedWeapon();

	UPROPERTY()
	AWeaponBase* SpawnedWeapon;

	void EquipWeapon(int32 _index);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "Weapon")
	void Server_EquipWeapon(int32 _index);

	UFUNCTION(Server, Reliable, WithValidation, Category = "Weapon")
	void PickupWeapon(TSubclassOf<AWeaponBase> _weaponPickup);

	UFUNCTION(Server, Reliable, WithValidation)
	void AddWeapon(TSubclassOf<AWeaponBase> _weapon);


	TArray<TSubclassOf<AWeaponBase>> GetWeaponInventory();

#pragma endregion

};
