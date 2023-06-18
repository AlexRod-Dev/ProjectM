// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "HealthPickup.generated.h"

UCLASS()
class PROJECTM_API AHealthPickup : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHealthPickup();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	float _healthAmount;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Sphere component used to test collision.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USphereComponent* SphereComponent;

	// Static Mesh used to provide a visual representation of the object.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UStaticMeshComponent* StaticMesh;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RecoverHp(APlayerBase* _playerCharacter);

private:
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                             const FHitResult& SweepResult);
};
