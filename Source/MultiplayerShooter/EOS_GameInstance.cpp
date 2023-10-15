// Fill out your copyright notice in the Description page of Project Settings.


#include "EOS_GameInstance.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Online/CoreOnline.h"
#include "Kismet/GameplayStatics.h"

//Getter utility
//#include "MyUtility.h"

//ServerBrowser to populate
#include "MyServerBrowser.h"

#include "MyMenuPlayerController.h"

void UEOS_GameInstance::StartGameInstance()
{
	Super::StartGameInstance();

}

void UEOS_GameInstance::loginWithEOS(FString ID, FString Token, FString LoginType) {
	//Setting up online subsystem
	IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(this->GetWorld());
	if (SubsystemRef == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("Null found at subsystemref"));
		return;
	}
	IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface();
	if (IdentityPointerRef != nullptr) {
		FOnlineAccountCredentials AccountDetails;
		AccountDetails.Id = ID;
		AccountDetails.Token = Token;
		AccountDetails.Type = LoginType;
		IdentityPointerRef->OnLoginCompleteDelegates->AddUObject(this, &UEOS_GameInstance::LoginWIthEOS_Return);
		IdentityPointerRef->Login(0, AccountDetails);

	}
}


void UEOS_GameInstance::LoginWIthEOS_Return(int32 LocalUserNum, bool bWasSuccess, const FUniqueNetId& UserId, const FString& Error)
{
	if (bWasSuccess) {
		UE_LOG(LogTemp, Warning, TEXT("Login Success"));
	}
	if (!bWasSuccess) {
		UE_LOG(LogTemp, Error, TEXT("Login Fail Reason - %S"), *Error);
	}
}


FString UEOS_GameInstance::getPlayerUsername()
//Returns player username 
{
	IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(this->GetWorld());
	if (SubsystemRef == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("Null found at subsystemref"));
		return FString();
	}
	IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface();
	if (IdentityPointerRef == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("Null found at IdentityPointer"));
		return FString();
	}
	if (IdentityPointerRef->GetLoginStatus(0) == ELoginStatus::LoggedIn) {
		return IdentityPointerRef->GetPlayerNickname(0);
	}
	return FString();

}

bool UEOS_GameInstance::isPlayerLoggedIn()
{
	IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(this->GetWorld());
	if (SubsystemRef == nullptr) {

		return 0;
	}
	IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface();
	if (IdentityPointerRef == nullptr) {

		return 0;
	}
	if (IdentityPointerRef->GetLoginStatus(0) == ELoginStatus::LoggedIn) {
		return 1;
	}
	return 0;
}

void UEOS_GameInstance::createEOSSession(bool bIsDedicatedServer, bool bIsLanServer, int32 NumberOfPublicConnections)
{
	IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(this->GetWorld());
	if (SubsystemRef == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("Null found at subsystemref"));
		return;
	}
	IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface();
	if (SessionPtrRef == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("Null found at subsystemref"));
		return;
	}
	FOnlineSessionSettings SessionCreationInfo;
	SessionCreationInfo.bIsDedicated = bIsDedicatedServer;
	SessionCreationInfo.bAllowInvites = true;
	SessionCreationInfo.bIsLANMatch = false;
	SessionCreationInfo.NumPublicConnections = NumberOfPublicConnections;
	SessionCreationInfo.bUseLobbiesIfAvailable = false; //dedicated server doesn't support lobbies, use sessions / matchmaking 
	SessionCreationInfo.bUsesPresence = true;
	SessionCreationInfo.bShouldAdvertise = true;
	SessionCreationInfo.bAllowJoinInProgress = true;
	SessionCreationInfo.bAllowJoinViaPresence = true;

	SessionCreationInfo.Set(SEARCH_KEYWORDS, FString("AlertBallLobby"), EOnlineDataAdvertisementType::ViaOnlineService);
	SessionPtrRef->OnCreateSessionCompleteDelegates.AddUObject(this, &UEOS_GameInstance::onCreateSessionCompleted);
	SessionPtrRef->CreateSession(0, MySessionName, SessionCreationInfo);
}

void UEOS_GameInstance::onCreateSessionCompleted(FName SessionName, bool bWasSuccesful)
{
	if (!bWasSuccesful) {
		UE_LOG(LogTemp, Error, TEXT("UnableToCreateSession"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("SessionCreated"));
	GetWorld()->ServerTravel(OpenLevelText);
	//UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), LevelToOpen);
}


void UEOS_GameInstance::findSessionAndJoin()
{
	IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(this->GetWorld());
	if (SubsystemRef == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("Null found at subsystemref"));
		return;
	}
	IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface();
	if (SessionPtrRef)
	{
		SessionSearch = nullptr;
		SessionSearch = MakeShareable(new FOnlineSessionSearch());
		SessionSearch->QuerySettings.Set(SEARCH_KEYWORDS, FString("AlertBallLobby"), EOnlineComparisonOp::Equals);
		//SessionSearch->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);
		SessionSearch->bIsLanQuery = false;
		SessionSearch->MaxSearchResults = 20;
		//SessionSearch->QuerySettings.SearchParams.Empty();
		SessionPtrRef->OnFindSessionsCompleteDelegates.AddUObject(this, &UEOS_GameInstance::onFindSessionCompleted);
		SessionPtrRef->FindSessions(0, SessionSearch.ToSharedRef());
	}

}

void UEOS_GameInstance::joinSession(int32 Index)
{
	IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(this->GetWorld());
	if (SubsystemRef == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("Null found at subsystemref"));
		return;
	}
	IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface();
	if (SessionPtrRef)
	{
		if (SessionSearch->SearchResults.Num() > 0) {

			SessionPtrRef->OnJoinSessionCompleteDelegates.AddUObject(this, &UEOS_GameInstance::onJoinSessionCompleted);
			// 				for (FOnlineSessionSearchResult i : SessionSearch->SearchResults) {
			// 					if (i.)
			// 				}
			SessionPtrRef->JoinSession(0, MySessionName, SessionSearch->SearchResults[Index]);
			UE_LOG(LogTemp, Warning, TEXT("Joined Session %s"), *SessionSearch->SearchResults[Index].Session.SessionInfo->ToString());
		}
		else {
			//createEOSSession(false, false, 10);
			UE_LOG(LogTemp, Error, TEXT("UEOS_GameInstance::joinSession: No Sessions found"));
		}
	}
}

void UEOS_GameInstance::onFindSessionCompleted(bool bWasSuccess)
{
	if (bWasSuccess) {
		AMyMenuPlayerController* PlayerController = Cast<AMyMenuPlayerController>(GetWorld()->GetFirstPlayerController());
		if (PlayerController == nullptr) {
			return;
			UE_LOG(LogTemp, Error, TEXT("UEOS_GameInstance::onFindSessionCompleted: No PlayerController"));
		}
		if (PlayerController->BrowserInst == nullptr) {
			return;
			UE_LOG(LogTemp, Error, TEXT("UEOS_GameInstance::onFindSessionCompleted: No BrowserInst"));
		}
		PlayerController->BrowserInst->populateScrollBox(SessionSearch->SearchResults);

		//UMyBrowserWidget* ServerBrowser = UMyUtility::GetBrowserWidget(this);
		//ServerBrowser->populateScrollBox(SessionSearch->SearchResults);
	}
	else {
		//createEOSSession(false, false, 10); //create a session if you couldn't find a session (not sure if I want to keep this)
		UE_LOG(LogTemp, Error, TEXT("Find session unsuccessful"));
	}
}


void UEOS_GameInstance::onJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		if (APlayerController* PlayerControllerRef = UGameplayStatics::GetPlayerController(GetWorld(), 0))
		{
			FString JoinAddress;
			IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(this->GetWorld());
			if (SubsystemRef == nullptr) {
				UE_LOG(LogTemp, Error, TEXT("Null found at subsystemref"));
				return;
			}
			IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface();
			if (SessionPtrRef)
			{
				SessionPtrRef->GetResolvedConnectString(MySessionName, JoinAddress);
				UE_LOG(LogTemp, Warning, TEXT("JoinAddress is %s"), *JoinAddress);
				if (!JoinAddress.IsEmpty()) {
					UE_LOG(LogTemp, Warning, TEXT("ClientTravel"));
					PlayerControllerRef->ClientTravel(JoinAddress, ETravelType::TRAVEL_Absolute);

				}
				else {
					UE_LOG(LogTemp, Error, TEXT("No Join Address"));
				}
			}
		}
	}
}

void UEOS_GameInstance::destroySession()
{
	IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(this->GetWorld());
	if (SubsystemRef == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("UEOS_GameInstance::destroySession: Null found at subsystemref"));
		return;
	}
	IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface();
	if (SessionPtrRef == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("UEOS_GameInstance::destroySession: Null found at sessionptref"));
		return;
	}
	SessionPtrRef->OnDestroySessionCompleteDelegates.AddUObject(this, &UEOS_GameInstance::onDestroySessionCompleted);
	SessionPtrRef->DestroySession(MySessionName);

}

void UEOS_GameInstance::setLevelReference(FString& Reference)
{
	OpenLevelText = Reference;
}

void UEOS_GameInstance::onDestroySessionCompleted(FName SessionName, bool bWasSuccesful)
{
	if (bWasSuccesful) {
		UE_LOG(LogTemp, Verbose, TEXT("UEOS_GameInstance::onDestroySessionCompleted: SessionDestroyed"));
	}
}

