// Fill out your copyright notice in the Description page of Project Settings.


#include "MyServerBrowser.h"

#include "Components/Button.h"
#include "Components/ScrollBox.h"

#include "EOS_GameInstance.h"

#include "MyBrowserButton.h"

void UMyServerBrowser::NativeConstruct()
{
	if (BTN_Refresh == nullptr) {
		return;
	}
	BTN_Refresh->OnClicked.AddDynamic(this, &UMyServerBrowser::onRefresh);
}

void UMyServerBrowser::onRefresh()
{
	UEOS_GameInstance* GameInstance = Cast<UEOS_GameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance) {
		GameInstance->findSessionAndJoin();
	}
}

void UMyServerBrowser::createSessionEntry(FOnlineSessionSearchResult SessionResult, int32 index)
{
	if (!BrowserButtonAsset) {
		UE_LOG(LogTemp, Error, TEXT("UMyBrowserWidget::createSessionEntry No BrowserButton Assigned"));
		return;
	}
	UMyBrowserButton* BrowserButton = CreateWidget<UMyBrowserButton>(GetWorld(), BrowserButtonAsset);

	//calculating occupancy
	int32 OpenConnections = SessionResult.Session.NumOpenPrivateConnections;
	int32 TotalConnections = SessionResult.Session.SessionSettings.NumPublicConnections;
	FString Occupancy = FString::Printf(TEXT("%i/%i"), TotalConnections - OpenConnections, TotalConnections);

	//get username
	FString Username = SessionResult.Session.GetSessionIdStr();


	BrowserButton->setupButton(Occupancy, Username, index);
	if (ScrollBox == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("UMyBrowserWidget::createSessionEntry No ScrollBox"));
		return;
	}
	ScrollBox->AddChild(BrowserButton);
}

void UMyServerBrowser::populateScrollBox(TArray<FOnlineSessionSearchResult> SessionResults)
{
	ScrollBox->ClearChildren();
	int32 i = 0;
	for (FOnlineSessionSearchResult SessionResult : SessionResults) {
		createSessionEntry(SessionResult, i);
		i++;
	}
}