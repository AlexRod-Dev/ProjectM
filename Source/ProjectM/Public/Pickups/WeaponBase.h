// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.generated.h"


UCLASS()
class PROJECTM_API AWeaponBase : public AActor
{
	GENERATED_BODY()


public:
	AWeaponBase(UStaticMesh* InBaseMesh);
	AWeaponBase(const FObjectInitializer& ObjectInitializer);

	
protected:
	virtual void BeginPlay() override;

	

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float _damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float _fireRate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 _ammoCapacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float _reloadTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UStaticMeshComponent* WeaponMesh;

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Weapon")
	virtual void Fire();

	void Reload();


	
};
