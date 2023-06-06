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
	
	UPROPERTY(BlueprintReadOnly,Replicated)
	float _health;

	
	
private:
	UMaterialInterface* _redMaterial;
	UMaterialInterface* _yellowMaterial;
	UPROPERTY(ReplicatedUsing=OnRep_Material)
	UMaterialInterface* _currentColor;

	

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

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerChangeBoxColor(UMaterialInterface* _material);
	void ServerChangeBoxColor_Implementation(UMaterialInterface* _material);
	bool ServerChangeBoxColor_Validate(UMaterialInterface* _material);
	void ChangeBoxColor(ABoxBase* _box, UMaterialInterface* _material);

protected:

	/** Property replication */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_Material();
};
