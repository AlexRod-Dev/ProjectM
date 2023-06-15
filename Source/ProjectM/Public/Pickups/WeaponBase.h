// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Net/UnrealNetwork.h"
#include "WeaponBase.generated.h"


UCLASS()
class PROJECTM_API AWeaponBase : public AActor
{
	GENERATED_BODY()


public:
	AWeaponBase(USkeletalMesh* InBaseMesh);
	AWeaponBase(const FObjectInitializer& ObjectInitializer);

	
protected:
	virtual void BeginPlay() override;

	


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float _damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float _fireRate;
	
	UPROPERTY(Replicated,EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 _ammoCapacity;

	UPROPERTY(Replicated,EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 _currentAmmo;

	UFUNCTION(BlueprintPure, Category = "Weapon")
	// ReSharper disable once UnrealHeaderToolError
	int32 GetCurrentAmmo();
	
	UFUNCTION(BlueprintPure, Category = "Weapon")
	int32 GetMaxAmmo();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float _reloadTime;

	UPROPERTY(EditAnywhere, Category = "Health")
	UClass* BulletClass;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	USkeletalMeshComponent* WeaponMesh;

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Weapon")
	virtual void Fire(APlayerBase* _player, UWorld* _world);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Weapon")
	virtual void Reload();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
