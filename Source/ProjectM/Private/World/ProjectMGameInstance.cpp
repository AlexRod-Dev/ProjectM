// Fill out your copyright notice in the Description page of Project Settings.


#include "World/ProjectMGameInstance.h"

#include "kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Player/CharacterController.h"

UProjectMGameInstance::UProjectMGameInstance()
{
}

void UProjectMGameInstance::Init()
{
	Super::Init();
	
	if(IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get())
	{
		SessionInterface = Subsystem->GetSessionInterface();
	
		if(SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UProjectMGameInstance::OnCreateSessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UProjectMGameInstance::OnFindSessionComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UProjectMGameInstance::OnJoinSessionComplete);
		}
	}
}

void UProjectMGameInstance::OnCreateSessionComplete(FName SessionName, bool bSucceeded)
{
	if(bSucceeded)
	{
		GetWorld()->ServerTravel("/Game/Maps/TestMap?Listen");
	}
}

void UProjectMGameInstance::OnFindSessionComplete(bool bSucceeded)
{
			
	if(bSucceeded)
	{
		TArray<FOnlineSessionSearchResult> SearchResults = SessionSearch->SearchResults;
		
		UE_LOG(LogTemp,Warning, TEXT("Search Results, Server Count: %d"), SearchResults.Num());
			
		if(SearchResults.Num())
		{
			UE_LOG(LogTemp,Warning, TEXT("Search Results, Server Count: %d"), SearchResults.Num());
			//SearchResults[0] just to test the connection
			SessionInterface->JoinSession(0, FName("ProjectM Session"), SearchResults[0]);
		}
	}
}

void UProjectMGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	
	if(ACharacterController* _playerController = Cast<ACharacterController>(UGameplayStatics::GetPlayerController(GetWorld(),0)))
	{
		FString JoinAdress = "";
		SessionInterface->GetResolvedConnectString(SessionName, JoinAdress);
		if(JoinAdress != "")
		{
			_playerController->ClientTravel(JoinAdress, TRAVEL_Absolute);
		}
	}
}

void UProjectMGameInstance::CreateServer()
{
	if(SessionInterface.IsValid())
	{

		UE_LOG(LogTemp, Warning, TEXT("someone clicked CreateServer"));
		FOnlineSessionSettings SessionSettings;
		SessionSettings.bAllowJoinInProgress = true;
		SessionSettings.bIsDedicated = false;
		SessionSettings.bIsLANMatch = (IOnlineSubsystem::Get()->GetSubsystemName() != "NULL") ? false : true;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUsesPresence = true;
		SessionSettings.NumPublicConnections = 5;

		SessionInterface->CreateSession(0, FName("ProjectM Session"), SessionSettings);
	}
}

void UProjectMGameInstance::JoinServer()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL");
	SessionSearch->MaxSearchResults = 10000;
	SessionSearch->QuerySettings.Set("SEARCH_PRESENCE", true, EOnlineComparisonOp::Equals);

	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	
}
