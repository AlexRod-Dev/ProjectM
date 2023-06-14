// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CharacterController.h"
#include "BulletBase.generated.h"

UCLASS()
class PROJECTM_API ABulletBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABulletBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Destroyed() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Sphere component used to test collision.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	class USphereComponent* SphereComponent;

	 // Static Mesh used to provide a visual representation of the object.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    class UStaticMeshComponent* StaticMesh;

    // Movement component for handling projectile movement.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    class UProjectileMovementComponent* ProjectileMovementComponent;

    // Particle used when the projectile impacts against another object and explodes.
    UPROPERTY(EditAnywhere, Category = "Effects")
    class UParticleSystem* _bloodEffect;

	//The damage type and damage that will be done by this projectile
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
    TSubclassOf<class UDamageType> _damageType;

    //The damage dealt by this projectile.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Damage")
    float _damage;

	UPROPERTY(BlueprintReadWrite, Category = "Bullet")
	 ACharacterController* _instigatorController;

	void SetInitialVelocity(const FVector& _velocity);
	
protected:
	UFUNCTION(Category="Projectile")
		void OnProjectileImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
