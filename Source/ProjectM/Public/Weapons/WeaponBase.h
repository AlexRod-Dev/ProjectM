// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/AudioComponent.h"
#include "Net/UnrealNetwork.h"
#include "WeaponBase.generated.h"


UCLASS()
class PROJECTM_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	AWeaponBase(USkeletalMesh* InBaseMesh);
	AWeaponBase(const FObjectInitializer& ObjectInitializer);

	//virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	


public:
	//Weapon Proprieties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float _damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float _fireRate;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 _ammoCapacity;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 _currentAmmo;
	//
	
	UFUNCTION(BlueprintPure, Category = "Weapon")
	// ReSharper disable once UnrealHeaderToolError
	int32 GetCurrentAmmo();

	UFUNCTION(BlueprintPure, Category = "Weapon")
	int32 GetMaxAmmo();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float _reloadTime;

	bool bIsReloading;
	
	//UPROPERTY(Replicated)
	//float _timeSinceLastShot;
	
	UPROPERTY(EditAnywhere, Category = "Health")
	UClass* BulletClass;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	USkeletalMeshComponent* WeaponMesh;

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Weapon")
	virtual void Fire(APlayerBase* _player, UWorld* _world,float _timeSinceLastShot);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Weapon")
	virtual void Reload();

	//Sounds
	UPROPERTY(EditAnywhere, Category = "Audio")
	class USoundCue* FireSound;

	UPROPERTY(EditAnywhere, Category = "Audio")
	class USoundCue* ReloadSound;

	UPROPERTY(Replicated)
	UAudioComponent* AudioComponent;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_PlaySound(USoundCue* _sound, FVector _location, UWorld* _world);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlaySound(USoundCue* _sound, FVector _location, UWorld* _world);

	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
