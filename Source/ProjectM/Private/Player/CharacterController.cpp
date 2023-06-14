	// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CharacterController.h"

#include "Player/BoxBase.h"
#include "GameFramework/Pawn.h"
#include "Pickups/WeaponBase.h"
#include "Player/BulletBase.h"
#include "Player/PlayerBase.h"
#include "World/ProjectMGameStateBase.h"
#include "Player/ProjectMPlayerState.h"

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

	_spawnDistance = 100.f;
	// Set the health pickup class
	static ConstructorHelpers::FClassFinder<ABoxBase> BoxClassFinder(TEXT("/Game/Blueprints/Objects/BP_BoxBase"));
	if (BoxClassFinder.Succeeded())
	{
		BoxClass = BoxClassFinder.Class;
	}
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

	//Bind Use Item button
	InputComponent->BindAction("UseItem", IE_Pressed,this, &ACharacterController::ServerSpawnBox);

	InputComponent->BindAction("PreviousWeap", IE_Pressed, this, &ACharacterController::PreviousWeapon);
	InputComponent->BindAction("NextWeap", IE_Pressed, this, &ACharacterController::NextWeapon);
	
	

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
		ServerShoot();

	}

}

void ACharacterController::StopShoot()
{
	bIsFiringWeapon = false;

}

	void ACharacterController::ServerSpawnBox_Implementation()
	{
	APawn* _playerPawn = GetPawn();
	if(_playerPawn)
	{
		FVector _spawnLocation = _playerPawn->GetActorLocation() + _playerPawn->GetActorForwardVector() * _spawnDistance;
		FRotator _spawnRotation = FRotator(1,1,1);

		FActorSpawnParameters _spawnParams;
		_spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		GetWorld()->SpawnActor<ABoxBase>(BoxClass, _spawnLocation,_spawnRotation, _spawnParams);
	}
	
	
}

void ACharacterController::PreviousWeapon()
{
	APawn* _playerPawn = GetPawn();

	if(_playerPawn)
	{
		APlayerBase* _player = Cast<APlayerBase>(_playerPawn);

		if(_player)
		{
			if(_player->GetCurrentWeapIndex() == 0)
			{
				_player->Server_EquipWeapon(_player->_weaponInventory.Num()-1);
				
			}else
			{
				_player->Server_EquipWeapon(_player->GetCurrentWeapIndex()-1);
			}
		}
	}
	
	
}

void ACharacterController::NextWeapon()
{
	APawn* _playerPawn = GetPawn();

	if(_playerPawn)
	{
		APlayerBase* _player = Cast<APlayerBase>(_playerPawn);

		if(_player)
		{
			if(_player->GetCurrentWeapIndex() >= _player->_weaponInventory.Num()-1)
			{
				_player->Server_EquipWeapon(0);
				
			}else
			{
				_player->Server_EquipWeapon(_player->GetCurrentWeapIndex()+1);
			}
		}
	}
}



	bool ACharacterController::ServerSpawnBox_Validate()
	{
		return true;
	}

	void ACharacterController::ServerShoot_Implementation()
{
	APawn* const _playerPawn = GetPawn();

	if(_playerPawn != nullptr)
	{
		APlayerBase* _player = Cast<APlayerBase>(_playerPawn);
		if(_player)
		{
			TSubclassOf<AWeaponBase> _weapon = _player->_equippedWeapon;
			if(_weapon)
				
				_weapon->GetDefaultObject<AWeaponBase>()->Fire(_player,GetWorld());
		}
		
		// FVector _spawnLocation = _playerPawn->GetActorLocation() + (_playerPawn->GetActorForwardVector() * 50.0f);
		// FRotator _spawnRotation = _playerPawn->GetActorRotation();
		//
		// FActorSpawnParameters _spawnParameters;
		// _spawnParameters.Instigator = GetInstigator();
		// _spawnParameters.Owner = this;
		//
		// ABulletBase* _spawnedBullet = GetWorld()->SpawnActor<ABulletBase>(_spawnLocation, _spawnRotation, _spawnParameters);
		//
		// 	if(_spawnedBullet)
		// 	{
		// 		_spawnedBullet->_instigatorController =	Cast<ACharacterController>(this);
		// 	}

	}
	
	
}



