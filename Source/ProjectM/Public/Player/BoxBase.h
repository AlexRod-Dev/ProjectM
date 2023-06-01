// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoxBase.generated.h"

UCLASS()
class PROJECTM_API ABoxBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoxBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

private:
	
	UPROPERTY(Replicated)
	float _health;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Static Mesh used to provide a visual representation of the object.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
	UStaticMeshComponent* StaticMesh;

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerDestroyBox();
	bool ServerDestroyBox_Validate();
	void ServerDestroyBox_Implementation();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerTakeDamage(float _damage);
	bool ServerTakeDamage_Validate(float _damage);
	void ServerTakeDamage_Implementation(float _damage);
	
	/** Property replication */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
