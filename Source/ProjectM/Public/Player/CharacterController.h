// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "Net/UnrealNetwork.h"
#include "CharacterController.generated.h"

UCLASS()
class PROJECTM_API ACharacterController : public APlayerController
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACharacterController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Health")
	UClass* BoxClass;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


#pragma region Inputs
	// Called to bind functionality to input
	virtual void SetupInputComponent() override;

	void MoveForward(float AxisValue);

	void MoveRight(float AxisValue);

	void DisableControls();

	void EnableControls();

#pragma endregion


	bool GetIsAlive();

	bool bIsAlive;

	UPROPERTY(Replicated)
	float _timeSinceLastShot;


#pragma region reload


	UPROPERTY(ReplicatedUsing = OnRep_IsReloading)
	bool bIsReloading;

	UPROPERTY(ReplicatedUsing = OnRep_ReloadTimer)
	float _reloadTimer;

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStartReload();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void StartReload();

	FTimerHandle TimerHandle_Reload;
	UFUNCTION()
	void OnRep_IsReloading();

	UFUNCTION()
	void OnRep_ReloadTimer();

	UFUNCTION()
	void ReloadComplete();


#pragma endregion

protected:
	UPROPERTY(EditDefaultsOnly, Category="Gameplay|Projectile")
	TSubclassOf<class ABulletBase> _projectileClass;

	// Delay between shots in seconds
	UPROPERTY(EditDefaultsOnly, Category="Gameplay")
	float _fireRate;

	// If true, you are in the process of firing projectiles.
	bool bIsFiringWeapon;


	// Server function for spawning projectiles.
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerShoot();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSpawnBox();


	void PreviousWeapon();

	void NextWeapon();


	UPROPERTY(EditAnywhere)
	float _spawnBoxDistance;


	/** Property replication */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
