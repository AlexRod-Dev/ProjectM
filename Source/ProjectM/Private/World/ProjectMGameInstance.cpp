// Fill out your copyright notice in the Description page of Project Settings.


#include "World/ProjectMGameInstance.h"

#include "kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Player/CharacterController.h"

UProjectMGameInstance::UProjectMGameInstance()
{
	MySessionName = FName("My Session");
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

	SearchingForServer.Broadcast(false);
	
	if(bSucceeded)
	{
		int32 ArrayIndex = -1;
		
	
		for(FOnlineSessionSearchResult Result : SessionSearch->SearchResults)
		{
			++ArrayIndex;
			if(!Result.IsValid())
				continue;

			FServerInfo Info;
			FString ServerName = "Empty Server Name";
			FString HostName = "Empty Host Name";

			Result.Session.SessionSettings.Get(FName("SERVER_NAME_KEY"), ServerName);
			Result.Session.SessionSettings.Get(FName("SERVER_HOSTNAME_KEY"), HostName);
			
			Info.ServerName = ServerName;
			Info.MaxPlayers = Result.Session.SessionSettings.NumPublicConnections;
			Info.CurrentPlayers = Info.MaxPlayers - Result.Session.NumOpenPublicConnections;
			Info.ServerArrayIndex = ArrayIndex;
			Info.SetPlayerCount();
			ServerListDel.Broadcast(Info);
			
	
		}
		
		 UE_LOG(LogTemp,Warning, TEXT("Search Results, Server Count: %d"), SessionSearch->SearchResults.Num());
		
		if(SessionSearch->SearchResults.Num())
		{
		//	UE_LOG(LogTemp,Warning, TEXT("Search Results, Server Count: %d"), SearchResults.Num());
			//SearchResults[0] just to test the connection
		//	SessionInterface->JoinSession(0, FName("ProjectM Session"), SearchResults[0]);
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

void UProjectMGameInstance::CreateServer(FString ServerName, FString HostName)
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

		SessionSettings.Set(FName("SERVER_NAME_KEY"), ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		SessionSettings.Set(FName("SERVER_HOSTNAME_KEY"), HostName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		
		SessionInterface->CreateSession(0, MySessionName, SessionSettings);
	}
}

void UProjectMGameInstance::FindServers()
{

	SearchingForServer.Broadcast(true);
	
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL");
	SessionSearch->MaxSearchResults = 10000;
	SessionSearch->QuerySettings.Set("SEARCH_PRESENCE", true, EOnlineComparisonOp::Equals);

	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	
}

void UProjectMGameInstance::JoinServer(int32 ArrayIndex)
{
	UE_LOG(LogTemp,Warning, TEXT("trying to join server at index: %d "), ArrayIndex);
	if(ArrayIndex >= 0 && ArrayIndex < SessionSearch->SearchResults.Num())
	{
		FOnlineSessionSearchResult Result = SessionSearch->SearchResults[ArrayIndex];
		if(Result.IsValid())
		{
			SessionInterface->JoinSession(0,MySessionName, Result);	
		}
		else
		{
			UE_LOG(LogTemp,Warning, TEXT("Failed to join server at index: %d "), ArrayIndex);
		}
		
	}

}
