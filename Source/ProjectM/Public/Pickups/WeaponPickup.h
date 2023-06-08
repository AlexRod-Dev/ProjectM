// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "WeaponPickup.generated.h"

UCLASS()
class PROJECTM_API AWeaponPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponPickup();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	TSubclassOf<AWeaponBase> WeaponClass;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	USphereComponent* SphereComponent;

	UPROPERTY(EditAnywhere, Category = "Pickup")
	UStaticMeshComponent* StaticMesh;

private:
	
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
