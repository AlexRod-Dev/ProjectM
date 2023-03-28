// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "PlayerBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTM_API APlayerBase : public APaperCharacter
{
	GENERATED_BODY()

	
public:
	APlayerBase();

	

protected:
	void BeginPlay() override;

public:
	void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool bAddDefaultMovementBindings;

private:

	
	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(Replicated, EditAnywhere)
	FVector MovementInput;

	UPROPERTY(EditAnywhere)
	float MoveSpeed;

	UFUNCTION(Server, Reliable)
	void Server_SetMovementInput(const FVector& Input);

	UFUNCTION(Client, Reliable)
	void Client_SetMovementInput(const FVector& Input);

	void MoveRight(float Value);
	

	void MoveUp(float Value);

	void OnRep_MovementInput();

	void Server_SetMovementInput_Implementation(const FVector& Input);

	void Client_SetMovementInput_Implementation(const FVector& Input);


	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	
};
