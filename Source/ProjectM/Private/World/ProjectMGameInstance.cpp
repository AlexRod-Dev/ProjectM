// Fill out your copyright notice in the Description page of Project Settings.


#include "World/ProjectMGameInstance.h"

#include "kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Interfaces/ITargetDevice.h"
#include "Player/CharacterController.h"



UProjectMGameInstance::UProjectMGameInstance()
: DestroySessionCompleteDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(this, &ThisClass::OnDestroySessionCompleted))
{
	MySessionName = FName("My Session");
	PlayerID = 0;
	MaxSessionPlayers = 9;
}



void UProjectMGameInstance::Init()
{
	Super::Init();

	//Access OnlineSubsystem
	if(IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get())
	{
		//Access Session Interface
		SessionInterface = Subsystem->GetSessionInterface();
	
		if(SessionInterface.IsValid())
		{
			//Bind Delegates
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UProjectMGameInstance::OnCreateSessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UProjectMGameInstance::OnFindSessionComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UProjectMGameInstance::OnJoinSessionComplete);
			GetEngine()->OnNetworkFailure().AddUObject(this, &UProjectMGameInstance::HandleNetworkFailure);
		}
	}
}

void UProjectMGameInstance::CreateServer(FString ServerName)
{
	if(SessionInterface.IsValid())
	{

		UE_LOG(LogTemp, Warning, TEXT("someone clicked CreateServer"));
		FOnlineSessionSettings SessionSettings;

		
		//Check if we are using the NULL Subsystem for testing purposes
		// if (IOnlineSubsystem::Get()->GetSubsystemName() != "NULL")
		// {
		// 	SessionSettings.bIsLANMatch = false;
		// }
		// else
		// {
		// 	SessionSettings.bIsLANMatch = true;
		// }


	//Set all the settings needed for the session
		//Check if we are using the NULL Subsystem for testing purposes
		//SessionSettings.bIsLANMatch = (IOnlineSubsystem::Get()->GetSubsystemName() != "NULL") ? false : true;
		if (IOnlineSubsystem::Get()->GetSubsystemName() != "NULL")
		{
			SessionSettings.bIsLANMatch = false;
		}
		else
		{
			SessionSettings.bIsLANMatch = true;
		}
		SessionSettings.bAllowJoinInProgress = true;
		SessionSettings.bIsDedicated = false;
		SessionSettings.bUseLobbiesIfAvailable = true;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUsesPresence = true;
		SessionSettings.NumPublicConnections = MaxSessionPlayers;

		//Set keys for the sever and host name
		SessionSettings.Set(FName("SERVER_NAME_KEY"), ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	
		//CreateSession and fires the delegate
		SessionInterface->CreateSession(PlayerID, MySessionName, SessionSettings);
		UE_LOG(LogTemp, Warning, TEXT("Session created by %s"), *MySessionName.ToString());

	}
}

void UProjectMGameInstance::OnCreateSessionComplete(FName SessionName, bool bSucceeded)
{
	UE_LOG(LogTemp, Warning, TEXT("OnCreate Session, Succeeded: %d"), bSucceeded);
	//Check if the session creation is succeeded
	if(bSucceeded)
	{
		GetWorld()->ServerTravel("/Game/Maps/Lobby?listen");
	}
}

void UProjectMGameInstance::FindServers()
{
	//Check if we are searching for servers
	SearchingForServer.Broadcast(true);
	
	SessionSearch = MakeShareable(new FOnlineSessionSearch());

	//Check if we are using Null subsystem for testing purposes
	if (IOnlineSubsystem::Get()->GetSubsystemName() != "NULL")
	{
		SessionSearch->bIsLanQuery = false;
	}
	else
	{
		SessionSearch->bIsLanQuery = true;
	}
	//SessionSearch->bIsLanQuery = (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL");
	//SessionSearch->bIsLanQuery = false;
	//Set the max Search Results when searching for sessions
	 UE_LOG(LogTemp,Warning, TEXT("Search as Lan?: %d"), SessionSearch->bIsLanQuery);
    		
	SessionSearch->MaxSearchResults = 10000;
	SessionSearch->QuerySettings.Set("SEARCH_PRESENCE", true, EOnlineComparisonOp::Equals);

	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	
}

void UProjectMGameInstance::OnFindSessionComplete(bool bSucceeded)
{
	//Check if we searched for servers
	SearchingForServer.Broadcast(false);
	
	if(bSucceeded)
	{
		int32 ArrayIndex = -1;
		
	
		for(FOnlineSessionSearchResult Result : SessionSearch->SearchResults)
		{
			++ArrayIndex;
			
			if(!Result.IsValid())
			{
				continue;
			}
				
			FServerInfo Info;

			//Set default server
			FString ServerName = "Empty Server Name";
			FString HostName = "Empty Host Name";

			//Get Server name from the session creator
			Result.Session.SessionSettings.Get(FName("SERVER_NAME_KEY"), ServerName);
			Result.Session.SessionSettings.Get(FName("SERVER_HOSTNAME_KEY"), HostName);

			//Use server search info
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

void UProjectMGameInstance::DestroySession()
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
	 
	
		if (!Sessions.IsValid())
		{
			OnDestroySessionCompleteEvent.Broadcast(false);
			return;
		}

		DestroySessionCompleteDelegateHandle =
			Sessions->AddOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegate);

		if (!Sessions->DestroySession(MySessionName))
		{
			Sessions->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);

			OnDestroySessionCompleteEvent.Broadcast(false);
		}
	}
}

void UProjectMGameInstance::OnDestroySessionCompleted(FName SessionName, bool Successful)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		
		if (Sessions)
		{
			Sessions->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
		}

		OnDestroySessionCompleteEvent.Broadcast(Successful);
	}
}

void UProjectMGameInstance::HandleNetworkFailure(UWorld* World, UNetDriver* NetDriver,
	ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	UGameplayStatics::OpenLevel(GetWorld(), "Lobby", true);

}

void UProjectMGameInstance::StartGame()
{
		GetWorld()->ServerTravel("/Game/Maps/TestMap?listen");
}

void UProjectMGameInstance::BackToLobby()
{
	GetWorld()->ServerTravel("/Game/Maps/Lobby?listen");
}

void UProjectMGameInstance::BackToStart(ACharacterController* _playerController)
{
	if(_playerController != nullptr)
	{
		_playerController->ClientTravel("/Game/Maps/StartMap", TRAVEL_Absolute);
	}
	
}






