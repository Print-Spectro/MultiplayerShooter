// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBrowserButton.h"

//widget includes
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "MyServerBrowser.h"
#include "EOS_GameInstance.h"

//utility

void UMyBrowserButton::NativeConstruct()
{
	Super::NativeConstruct();
	if (BTN_Server) {
		BTN_Server->OnClicked.AddDynamic(this, &UMyBrowserButton::onButtonPress);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("UMyBrowserButton::NativeConstruct: Null at BTN_Server, likely you assigned button class instead of the blueprint"))
	}

}

void UMyBrowserButton::setupButton(const FString Occupancy, const FString OwningPlayer, const int32 SessionIndex)
{
	if (TXT_OwningPlayer) {
		TXT_OwningPlayer->SetText(FText::FromString(OwningPlayer));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("UMyBrowserButton::setupButton: Null at TXT_OwningPlayer"))
	}
	if (TXT_OwningPlayer) {
		TXT_Occupancy->SetText(FText::FromString(Occupancy));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("UMyBrowserButton::setupButton: Null at TXT_Occupancy"))
	}


	SessionToJoin = SessionIndex;
}

//creating function for when this button instance is pressed. It sets the session index to join in the browser.
void UMyBrowserButton::onButtonPress()
{
	//UMyServerBrowser* Browser = Cast<UMyServerBrowser>(UMyUtility::GetBrowserWidget(this));
	//if (Browser == nullptr) {
	//	UE_LOG(LogTemp, Warning, TEXT("UMyBrowserButton::onButtonPress: Cast to BrowserWidgetFailed"))
	//		return;
	//}
	//Browser->setSessionToJoin(SessionToJoin);
	UEOS_GameInstance* GameInstance = Cast<UEOS_GameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance) {
		GameInstance->joinSession(SessionToJoin);
	}
}

