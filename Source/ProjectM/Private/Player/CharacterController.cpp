// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CharacterController.h"

#include "Player/BoxBase.h"
#include "GameFramework/Pawn.h"
#include "Weapons/WeaponBase.h"
#include "Player/BulletBase.h"
#include "Player/PlayerBase.h"
#include "Player/ProjectMPlayerState.h"
#include "Weapons/Pistol.h"

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

	_timeSinceLastShot = 0;

	bIsAlive = true;

	bIsReloading = false;
	
	_spawnBoxDistance = 100.f;
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

	_timeSinceLastShot += DeltaTime;
	
	// Get the player state from the controller and cast it to the custom class type
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
	InputComponent->BindAction("Fire", IE_Pressed, this, &ACharacterController::ServerShoot);

	InputComponent->BindAction("Reload", IE_Pressed, this, &ACharacterController::StartReload);
	//Bind Use Item button
	InputComponent->BindAction("UseItem", IE_Pressed, this, &ACharacterController::ServerSpawnBox);

	InputComponent->BindAction("PreviousWeap", IE_Pressed, this, &ACharacterController::PreviousWeapon);
	InputComponent->BindAction("NextWeap", IE_Pressed, this, &ACharacterController::NextWeapon);
}

void ACharacterController::MoveForward(float AxisValue)
{
	if (AxisValue != 0.0f)
	{
		APawn* const _playerPawn = GetPawn();
		if (_playerPawn != nullptr)
		{
			_playerPawn->AddMovementInput(FVector(1.0f, 0.0f, 0.0f), AxisValue);
		}
	}
}

void ACharacterController::MoveRight(float AxisValue)
{
	if (AxisValue != 0.0f)
	{
		APawn* const _playerPawn = GetPawn();
		if (_playerPawn != nullptr)
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

void ACharacterController::StartReload()
{
		APawn* const _playerPawn = GetPawn();

		if (_playerPawn != nullptr)
		{
			APlayerBase* _player = Cast<APlayerBase>(_playerPawn);
			if (_player)
			{
				TSubclassOf<AWeaponBase> _weapon = _player->_equippedWeapon;
				if(_weapon != nullptr)
				{
					if(HasAuthority())
					{
						bIsReloading = true;
						
						//get reload time of current weapon
						_reloadTimer = _weapon->GetDefaultObject<AWeaponBase>()->_reloadTime;

						//Set timer to callreload complete
						GetWorld()->GetTimerManager().SetTimer(TimerHandle_Reload, this, &ACharacterController::ReloadComplete, _reloadTimer,false);
					}
					else
					{
						ServerStartReload();
					}
				}
			}
		}
}


void ACharacterController::ServerStartReload_Implementation()
{
	if(HasAuthority())
	{
		StartReload();
	}
}



void ACharacterController::OnRep_IsReloading()
{
}

void ACharacterController::OnRep_ReloadTimer()
{
}

void ACharacterController::ReloadComplete()
{
	if(HasAuthority())
	{
		bIsReloading = false;
		APawn* const _playerPawn = GetPawn();

		if (_playerPawn != nullptr)
		{
			APlayerBase* _player = Cast<APlayerBase>(_playerPawn);
			if (_player)
			{
				TSubclassOf<AWeaponBase> _weapon = _player->_equippedWeapon;
				if (_weapon)

				{
					_weapon->GetDefaultObject<AWeaponBase>()->Reload();
				}
			}
		}
	}
}

bool ACharacterController::ServerShoot_Validate()
{
	return true;
}


void ACharacterController::ServerShoot_Implementation()
{
	if(!bIsReloading)
	{
		APawn* const _playerPawn = GetPawn();

		if (_playerPawn != nullptr)
		{
			APlayerBase* _player = Cast<APlayerBase>(_playerPawn);
			if (_player)
			{
 			
				TSubclassOf<AWeaponBase> _weapon = _player->_equippedWeapon;
				if(_weapon != nullptr)
				{
					if(HasAuthority())
					{
						_weapon->GetDefaultObject<AWeaponBase>()->ServerFire(_player, GetWorld(),_timeSinceLastShot);
					
					}
					else
					{
						if(GetNetMode()==NM_Client)
						{
							_weapon->GetDefaultObject<AWeaponBase>()->ServerFire_Implementation(_player, GetWorld(),_timeSinceLastShot);
					
						}
					}
					
				}
			}
		}
	}
}








void ACharacterController::ServerSpawnBox_Implementation()
{
	APawn* _playerPawn = GetPawn();
	if (_playerPawn)
	{
		FVector _spawnLocation = _playerPawn->GetActorLocation() + _playerPawn->GetActorForwardVector() *
			_spawnBoxDistance;
		FRotator _spawnRotation = FRotator(1, 1, 1);

		FActorSpawnParameters _spawnParams;
		_spawnParams.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		GetWorld()->SpawnActor<ABoxBase>(BoxClass, _spawnLocation, _spawnRotation, _spawnParams);
	}
}

bool ACharacterController::ServerStartReload_Validate()
{
	return true;
}

void ACharacterController::PreviousWeapon()
{
	APawn* _playerPawn = GetPawn();

	if (_playerPawn)
	{
		APlayerBase* _player = Cast<APlayerBase>(_playerPawn);

		if (_player)
		{
			if (_player->GetCurrentWeapIndex() == 0)
			{
				_player->Server_EquipWeapon(_player->_weaponInventory.Num() - 1);
			}
			else
			{
				_player->Server_EquipWeapon(_player->GetCurrentWeapIndex() - 1);
			}
		}
	}
}

void ACharacterController::NextWeapon()
{
	APawn* _playerPawn = GetPawn();

	if (_playerPawn)
	{
		APlayerBase* _player = Cast<APlayerBase>(_playerPawn);

		if (_player)
		{
			if (_player->GetCurrentWeapIndex() >= _player->_weaponInventory.Num() - 1)
			{
				_player->Server_EquipWeapon(0);
			}
			else
			{
				_player->Server_EquipWeapon(_player->GetCurrentWeapIndex() + 1);
			}
		}
	}
}

void ACharacterController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACharacterController, _timeSinceLastShot);
	DOREPLIFETIME(ACharacterController, bIsReloading);
	DOREPLIFETIME(ACharacterController, _reloadTimer);
}


bool ACharacterController::ServerSpawnBox_Validate()
{
	return true;
}
