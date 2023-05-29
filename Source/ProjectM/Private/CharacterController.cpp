	// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterController.h"
#include "GameFramework/Pawn.h"
#include "BulletBase.h"
#include "PlayerBase.h"
#include "ProjectMGameStateBase.h"
#include "ProjectMPlayerState.h"

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

	bIsAlive = true;

}

// Called every frame
void ACharacterController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("Controller Name: %s | is Alive: %s "), *GetName(),(GetIsAlive() ? TEXT("True") : TEXT("False")));

	// Get the player state from the controller and cast it to our custom class type
	AProjectMPlayerState* _playerState = Cast<AProjectMPlayerState>(GetPlayerState<AProjectMPlayerState>());

	// Check if the custom player state is valid
	if (_playerState != nullptr)
	{
		// Access the custom player state's properties or functions
		int32 Score = _playerState->GetScore();
		UE_LOG(LogTemp,Warning,TEXT("Score: %d"), Score);
		
	}
	
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
		APawn* const _playerPawn = GetPawn();
		if(_playerPawn != nullptr)
		{
			_playerPawn->AddMovementInput(FVector(1.0f,0.0f,0.0f), AxisValue);
		}
		
	}
}

void ACharacterController::MoveRight(float AxisValue)
{
	if(AxisValue != 0.0f)
	{
		APawn* const _playerPawn = GetPawn();
		if(_playerPawn != nullptr)
		{
			_playerPawn->AddMovementInput(FVector(0.0f, 1.0f, 0.0f), AxisValue);
		}
	}
}

void ACharacterController::DisableControls()
{
	DisableInput(this);
}

void ACharacterController::EnableControls()
{
	EnableInput(this);
}

	bool ACharacterController::GetIsAlive()
	{
	return bIsAlive;
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
	APawn* const _playerPawn = GetPawn();

	if(_playerPawn != nullptr)
	{
		FVector _spawnLocation = _playerPawn->GetActorLocation() + (_playerPawn->GetActorForwardVector() * 50.0f);
		FRotator _spawnRotation = _playerPawn->GetActorRotation();
	
		FActorSpawnParameters _spawnParameters;
		_spawnParameters.Instigator = GetInstigator();
		_spawnParameters.Owner = this;

		ABulletBase* _spawnedBullet = GetWorld()->SpawnActor<ABulletBase>(_spawnLocation, _spawnRotation, _spawnParameters);

			if(_spawnedBullet)
			{
				_spawnedBullet->_instigatorController =	Cast<ACharacterController>(this);
			}

	}
	
	
}



