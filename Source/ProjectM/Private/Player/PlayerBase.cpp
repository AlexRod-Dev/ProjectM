// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Player/CharacterController.h"
#include "UObject/SoftObjectPtr.h"
#include "World/ProjectMGameModeBase.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Pickups/WeaponPickup.h"
#include "World/ProjectMGameStateBase.h"


APlayerBase::APlayerBase()
{
	//Stats

	//400.0f = 400 cm/s
	_maxMoveSpeed = 400.0f;

	_moveSpeed = _maxMoveSpeed;

	_maxHealth = 100.0f;

	_currentHealth = _maxHealth;

	bIsDead = false;

	_respawnTime = 5.f;

	//CountdownWidget = nullptr;

	//Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 2500.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	GetCharacterMovement()->MaxWalkSpeed = _moveSpeed;

	GetMesh()->SetupAttachment(GetRootComponent());

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

// Called when the game starts or when spawned
void APlayerBase::BeginPlay()
{
	Super::BeginPlay();

	_currentWeapIndex = 0;

	_equippedWeapon = _weaponInventory[0];

	if (_equippedWeapon != nullptr)
	{
		if (HasAuthority())
		{
			// Spawn the pistol at the player's hand socket
			SpawnedWeapon = GetWorld()->SpawnActor<AWeaponBase>(_equippedWeapon,
			                                                    GetMesh()->GetSocketLocation("WeaponSocket"),
			                                                    GetMesh()->GetSocketRotation("WeaponSocket"));

			// Attach the pistol to the player's hand socket
			SpawnedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			                                 "WeaponSocket");
			
		}
	}
	
	// //Create instance of the widget
	// if(CountdownWidget)
	// {
	// 	WidgetInstance = CreateWidget<UUserWidget>(GetWorld(),CountdownWidget);
	//
	// 	if(WidgetInstance)
	// 	{
	// 		//Add widget to viewport
	// 		WidgetInstance->AddToViewport();
	// 		//Hide the widget since player is not dead
	// 		WidgetInstance->SetVisibility(ESlateVisibility::Hidden);
	// 	}
	// }
}

void APlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


// Replicated Properties
void APlayerBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Replicate current health.
	DOREPLIFETIME(APlayerBase, _currentHealth);
	DOREPLIFETIME(APlayerBase, _weaponInventory);
	DOREPLIFETIME(APlayerBase, _equippedWeapon);
	DOREPLIFETIME(APlayerBase, _currentWeapIndex);
	DOREPLIFETIME(APlayerBase, _respawnCountdown);
}

void APlayerBase::OnHealthUpdate()
{
	//All machines
	if (_currentHealth <= 0)
	{
		Die();
	}
}


float APlayerBase::TakeDamage(float _damageTaken, const FDamageEvent& DamageEvent, AController* EventInstigator,
                              AActor* _otherActor)
{
	float _damageApplied = _currentHealth - _damageTaken;
	SetCurrentHealth(_damageApplied);

	return _damageApplied;
}

float APlayerBase::RecoverHealth(float _amount)
{
	float _healthApplied = _currentHealth + _amount;
	SetCurrentHealth(_healthApplied);

	return _healthApplied;
}


void APlayerBase::SetCurrentHealth(float _hpValue)
{
	if (HasAuthority())
	{
		_currentHealth = FMath::Clamp(_hpValue, 0.f, _maxHealth);
		OnHealthUpdate();
	}
}


void APlayerBase::Die()
{
	bIsDead = true;
	
	AController* _controller = GetInstigatorController();
	ACharacterController* _playerController = Cast<ACharacterController>(_controller);
	
	if (_playerController != nullptr)
	{
		//In order for the update to only count 1 time
		if(_playerController->bIsAlive == true)
		{
			UWorld* _world = GetWorld();
			if(_world != nullptr)
			{

				AProjectMGameStateBase* _gameState = Cast<AProjectMGameStateBase>(_world->GetGameState());

				if(_gameState != nullptr)
				{
					_gameState->UpdatePlayersAlive(-1);

						if(_gameState->_playersAlive <= 0)
						{
							_gameState->ShowEndGameWidget();
						}
				}
			}
		}
		_playerController->bIsAlive = false;
	}

	if (HasAuthority())
	{
		MultiDie();
		
		//Destroy equipped weapon
		SpawnedWeapon->Destroy();

		_respawnCountdown = _respawnTime;
		
		//start timer to respawn
		//GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &APlayerBase::HandleRespawnTimer, 5.f, false);

		//Start Timer for countdown
		GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle,this, &APlayerBase::UpdateRespawnCountdown, 1.f, true);
		_playerController->ToggleRespawnCountdown(bIsDead);
	}
}

void APlayerBase::MultiDie_Implementation()
{
	//Ragdoll
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
	//Destroy equipped weapon
	SpawnedWeapon->Destroy();
}

void APlayerBase::Respawn()
{
	bIsDead = false;

	AController* _controller = GetController();
	AGameModeBase* GM = GetWorld()->GetAuthGameMode();

	ACharacterController* _playerController = Cast<ACharacterController>(_controller);

	_playerController->ToggleRespawnCountdown(bIsDead);
	

	if (AProjectMGameModeBase* _gameMode = Cast<AProjectMGameModeBase>(GM))
	{
		_gameMode->Respawn(_playerController);
		
		Destroy();
	}
}


void APlayerBase::HandleRespawnTimer()
{
	Respawn();
}

void APlayerBase::UpdateRespawnCountdown()
{
	
	//Decrement the respawn countdown
	_respawnCountdown -= 1.0f;

	if(_respawnCountdown <= 0.0f)
	{
		GetWorldTimerManager().ClearTimer(RespawnTimerHandle);

		Respawn();
	}
	
}


void APlayerBase::AddWeapon_Implementation(TSubclassOf<AWeaponBase> _weapon)
{
	if (_weapon)
	{
		if (HasAuthority())
		{
			_weaponInventory.Add(_weapon);
		}
	}
}


TArray<TSubclassOf<AWeaponBase>> APlayerBase::GetWeaponInventory()
{
	return _weaponInventory;
}

void APlayerBase::PickupWeapon(TSubclassOf<AWeaponBase> _weaponPickup)
{
	if (HasAuthority())
	{
		if (_weaponPickup)
		{
			//If weapon is already on inventory
			if (!_weaponInventory.Contains(_weaponPickup))
			{
				AddWeapon(_weaponPickup);
			}
			else
			{
				//Add ammo to current weapon
				for (TSubclassOf<AWeaponBase> _pickedWeapon : _weaponInventory)
				{
					if (_pickedWeapon == _weaponPickup)
					{
						_pickedWeapon->GetDefaultObject<AWeaponBase>()->ServerAddAmmo();
						return;
					}
				}
			}
		}
	}
	else
	{
		ServerPickupWeapon(_weaponPickup);
	}
}

void APlayerBase::ServerPickupWeapon_Implementation(TSubclassOf<AWeaponBase> _weaponPickup)
{
	PickupWeapon(_weaponPickup);
}

void APlayerBase::OnRep_CurrentHealth()
{
	OnHealthUpdate();
}

int32 APlayerBase::GetCurrentWeapIndex()
{
	return _currentWeapIndex;
}

TSubclassOf<AWeaponBase> APlayerBase::GetEquippedWeapon()
{
	return _equippedWeapon;
}

void APlayerBase::OnRep_EquippedWeapon()
{
	EquipWeapon(_currentWeapIndex);
}


void APlayerBase::EquipWeapon(int32 _index)
{
	//Server_EquipWeapon(_index);


	if (_weaponInventory.IsValidIndex(_index))
	{
		_currentWeapIndex = _index;
		_equippedWeapon = _weaponInventory[_index];

		if (SpawnedWeapon)
		{
			SpawnedWeapon->Destroy();
		}
		
		// Spawn the pistol at the player's hand socket
		SpawnedWeapon = GetWorld()->SpawnActor<AWeaponBase>(_equippedWeapon,
		                                                    GetMesh()->GetSocketLocation("WeaponSocket"),
		                                                    GetMesh()->GetSocketRotation("WeaponSocket"));

		// Attach the pistol to the player's hand socket
		SpawnedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		                                 "WeaponSocket");
	}
}

void APlayerBase::Server_EquipWeapon_Implementation(int32 _index)
{
	if (_weaponInventory.IsValidIndex(_index))
	{
		if (HasAuthority())
		{
			_currentWeapIndex = _index;
			_equippedWeapon = _weaponInventory[_index];


			if (SpawnedWeapon)
			{
				SpawnedWeapon->Destroy();
			}

			if (_equippedWeapon != nullptr)
			{
				// Spawn the pistol at the player's hand socket
				SpawnedWeapon = GetWorld()->SpawnActor<AWeaponBase>(_equippedWeapon,
				                                                    GetMesh()->GetSocketLocation("WeaponSocket"),
				                                                    GetMesh()->GetSocketRotation("WeaponSocket"));

				// Attach the pistol to the player's hand socket
				SpawnedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				                                 "WeaponSocket");
			}
		}
	}
}


bool APlayerBase::AddWeapon_Validate(TSubclassOf<AWeaponBase> _weapon)
{
	return true;
}

bool APlayerBase::ServerPickupWeapon_Validate(TSubclassOf<AWeaponBase> _weaponPickup)
{
	return true;
}

bool APlayerBase::Server_EquipWeapon_Validate(int32 _index)
{
	return true;
}
