	// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterController.h"
#include "GameFramework/Pawn.h"
#include "PlayerBase.h"

// Sets default values
ACharacterController::ACharacterController()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//Initialize projectile class
	_projectileClass = ABulletBase::StaticClass();

	//Initialize fire rate
	_fireRate = 0.15f;

	bIsFiringWeapon = false;

}

// Called when the game starts or when spawned
void ACharacterController::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACharacterController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();


	//Bind movement buttons to actions(function)
	InputComponent->BindAxis("MoveForward", this, &ACharacterController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ACharacterController::MoveRight);

	//Bind Fire Button to Action(function)
	InputComponent->BindAction("Fire", IE_Pressed, this, &ACharacterController::StartShoot);


}

void ACharacterController::MoveForward(float AxisValue)
{
	if(AxisValue != 0.0f)
	{
		APawn* const PlayerPawn = GetPawn();
		PlayerPawn->AddMovementInput(FVector(1.0f,0.0f,0.0f), AxisValue);
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%f = AxisValue"), AxisValue));
	
		
	}
}

void ACharacterController::MoveRight(float AxisValue)
{
	if(AxisValue != 0.0f)
	{
		APawn* const PlayerPawn = GetPawn();
		PlayerPawn->AddMovementInput(FVector(0.0f, 1.0f, 0.0f), AxisValue);
	}
}





void ACharacterController::StartShoot()
{

	if (!bIsFiringWeapon)
	{
		bIsFiringWeapon = true;
		UWorld* _world = GetWorld();
		_world->GetTimerManager().SetTimer(_firingTimer, this, &ACharacterController::StopShoot, _fireRate, false);
		HandleShoot();

	}

}

void ACharacterController::StopShoot()
{
	bIsFiringWeapon = false;

}

void ACharacterController::HandleShoot_Implementation()
{
	APawn* const PlayerPawn = GetPawn();

	FVector _spawnLocation = PlayerPawn->GetActorLocation() + (PlayerPawn->GetActorForwardVector() * 50.0f);
	FRotator _spawnRotation = PlayerPawn->GetActorRotation();
	
	FActorSpawnParameters _spawnParameters;
	_spawnParameters.Instigator = GetInstigator();
	_spawnParameters.Owner = this;

	ABulletBase* _spawnedBullet = GetWorld()->SpawnActor<ABulletBase>(_spawnLocation, _spawnRotation, _spawnParameters);
	

}

