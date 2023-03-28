// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"


APlayerBase::APlayerBase()
{

	//Set up boom arm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->TargetArmLength = 500.0f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SetupAttachment(RootComponent);

	//Set up Camera
	Camera = CreateDefaultSubobject <UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	//Set up New input system
	PrimaryActorTick.bCanEverTick = true;
	bAddDefaultMovementBindings = false;

}

void APlayerBase::BeginPlay(){

	Super::BeginPlay();

	if (HasAuthority())
	{
		// Spawn the player character on the server.
		APlayerBase* NewPlayer = GetWorld()->SpawnActor<APlayerBase>(GetActorLocation(), GetActorRotation());
		NewPlayer->SetOwner(this);

		// Spawn a new player controller for the owning player.
		APlayerController* NewController = GetWorld()->SpawnActor<APlayerController>(GetActorLocation(), GetActorRotation());
		NewController->Possess(NewPlayer);

		// Replicate the spawned player character to clients.
		if (HasAuthority())
		{
			NewPlayer->SpawnActor<AActor>(FTransform::Identity, FActorSpawnParameters());
		}
	}
}

void APlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Update camera rotation based on controller rotation
	if(Controller != nullptr && SpringArm != nullptr)
	{
		SpringArm->SetWorldRotation(Controller->GetControlRotation());
	}
}

void APlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerBase::MoveRight);
	PlayerInputComponent->BindAxis("MoveUp", this, &APlayerBase::MoveUp);
}

void APlayerBase::MoveRight(float Value)
{
	MovementInput.X = Value;
	if (HasAuthority())
	{
		Server_SetMovementInput(MovementInput);
	}
	else
	{
		Client_SetMovementInput(MovementInput);
	}
	AddMovementInput(FVector(0.0f, 1.0f, 0.0f), Value);
}

void APlayerBase::MoveUp(float Value)
{
	MovementInput.Y = Value;
	if (HasAuthority())
	{
		Server_SetMovementInput(MovementInput);
	}
	else
	{
		Client_SetMovementInput(MovementInput);
	}
	AddMovementInput(FVector(-1.0f, 0.0f, 0.0f), Value);
}


void APlayerBase::Server_SetMovementInput_Implementation(const FVector& Input)
{
	MovementInput = Input;
}

void APlayerBase::Client_SetMovementInput_Implementation(const FVector& Input)
{
	MovementInput = Input;
}

void APlayerBase::OnRep_MovementInput()
{
	// Update the character movement based on the replicated movement input
	FVector Movement = FVector(MovementInput.Y, MovementInput.X, 0.0f);
	Movement.Normalize();
	AddMovementInput(Movement);
}

void APlayerBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}


