// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/PrimitiveComponent.h"
#include "CharacterController.h"
#include "UObject/SoftObjectPtr.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "../ProjectMGameModeBase.h"
#include "Engine/World.h"


APlayerBase::APlayerBase()
{

	//Stats
	
	//400.0f = 400 cm/s
	_maxMoveSpeed = 400.0f;

	_moveSpeed = _maxMoveSpeed;

	_maxHealth = 100.0f;

	_currentHealth = _maxHealth;

	bIsDead = false;


	//Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
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

}

void APlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


// Replicated Properties
void APlayerBase::GetLifetimeReplicatedProps(TArray <FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Replicate current health.
	DOREPLIFETIME(APlayerBase, _currentHealth);
}

void APlayerBase::OnHealthUpdate()
{

	
		////Client Specific functionality
		//if (IsLocallyControlled())
		//{
		//}

		////Server specific functionality
		//if (GetLocalRole() == ROLE_Authority)
		//{
		//}


		//All machines
		if (_currentHealth <= 0)
		{
			Die();
			
		}

	

}


float APlayerBase::TakeDamage(float _damageTaken, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* _otherActor)
{
	float _damageApplied = _currentHealth - _damageTaken;
	SetCurrentHealth(_damageApplied);

	return _damageApplied;
}


void APlayerBase::SetCurrentHealth(float _hpValue)
{
	if(GetLocalRole() == ROLE_Authority)
	{
		_currentHealth = FMath::Clamp(_hpValue, 0.f, _maxHealth);
		OnHealthUpdate();
	}
}



void APlayerBase::Die()
{
	


	//Disable Inputs
	AController* _controller = GetInstigatorController();
	ACharacterController* _playerController = Cast<ACharacterController>(_controller);

	if(_playerController != nullptr)
	{
		_playerController->bIsAlive = false;
	}
	// if (IsLocallyControlled())
	// {
	// 	_playerController->DisableControls();
	// }

	if(GetLocalRole() == ROLE_Authority)
	{
		MultiDie();

		//start timer to respawn
		GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &APlayerBase::HandleRespawnTimer, 5.f, false);

	}
	
}

void APlayerBase::MultiDie_Implementation()
{
	//Ragdoll
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);

	
}

void APlayerBase::Respawn()
{
	bIsDead = false;
	
	AController* _controller = GetController();
	AGameModeBase* GM = GetWorld()->GetAuthGameMode();

	ACharacterController* _playerController = Cast<ACharacterController>(_controller);

	//Enable Inputs (not working somehow)
	/*if(IsLocallyControlled())
	{
		_playerController->EnableControls();

	}*/

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

void APlayerBase::OnRep_CurrentHealth()
{
	OnHealthUpdate();
}






