// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "EOS_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSHOOTER_API UEOS_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	virtual void StartGameInstance() override;
	
public:
	UFUNCTION(BlueprintCallable, Category = "EOS Functions")
		void loginWithEOS(FString ID, FString Token, FString LoginType);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOS Functions")
		FString getPlayerUsername();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOS Functions")
		bool isPlayerLoggedIn();

	UFUNCTION(BlueprintCallable, Category = "EOS Functions")
		void createEOSSession(bool bIsDedicatedServer, bool bIsLanServer, int32 NumberOfPublicConnections);

	UFUNCTION(BlueprintCallable, Category = "EOS Functions")
		void findSessionAndJoin();

	UFUNCTION(BlueprintCallable, Category = "EOS Functions")
		void joinSession(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "EOS Functions")
		void destroySession();

	UFUNCTION()
		void setLevelReference(FString& Reference);


	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS Variables")
		FString OpenLevelText = FString("/Game/FirstPerson/Maps/FirstPersonMap?listen");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS Variables")
		TSoftObjectPtr<UWorld> LevelToOpen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS Variables")
		FName MySessionName = FName("MainSession");

	void onCreateSessionCompleted(FName SessionName, bool bWasSuccesful);
	void onDestroySessionCompleted(FName SessionName, bool bWasSuccesful);
	void LoginWIthEOS_Return(int32 LocalUserNum, bool bWasSuccess, const FUniqueNetId& UserId, const FString& Error);
	void onFindSessionCompleted(bool bWasSuccess);
	void onJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result); //Include OnlineSessionInterface



};
