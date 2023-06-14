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

	// Called to bind functionality to input
	virtual void SetupInputComponent() override;


	void MoveForward(float AxisValue);

	void MoveRight(float AxisValue);

	void DisableControls();

	void EnableControls();
	
	bool GetIsAlive();

	bool bIsAlive;


protected:
	
	UPROPERTY(EditDefaultsOnly, Category="Gameplay|Projectile")
    TSubclassOf<class ABulletBase> _projectileClass;
	
	// Delay between shots in seconds
    UPROPERTY(EditDefaultsOnly, Category="Gameplay")
    float _fireRate;

	// If true, you are in the process of firing projectiles.
	bool bIsFiringWeapon;

	// Function for beginning weapon fire.
	UFUNCTION(Category="Gameplay") 
	void StartShoot();

	// Function for ending weapon fire. Once this is called, the player can use StartFire again.
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
    void StopShoot();  

	// Server function for spawning projectiles.
    UFUNCTION(Server, Reliable)
    void ServerShoot();

	// A timer handle used for providing the fire rate delay in-between spawns.
	FTimerHandle _firingTimer;

	UFUNCTION(Server,Reliable,WithValidation)
	void ServerSpawnBox();
	bool ServerSpawnBox_Validate();
	void ServerSpawnBox_Implementation();

	void PreviousWeapon();
	
	void NextWeapon();
	
	
	UPROPERTY(EditAnywhere)
	float _spawnDistance;
};
