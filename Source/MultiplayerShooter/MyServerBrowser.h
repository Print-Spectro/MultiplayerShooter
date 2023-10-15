// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyServerBrowser.generated.h"


class UButton;
class UScrollBox;
/**
 * 
 */
UCLASS()
class MULTIPLAYERSHOOTER_API UMyServerBrowser : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

public:
	void populateScrollBox(TArray<FOnlineSessionSearchResult> SessionsResults);

	void createSessionEntry(FOnlineSessionSearchResult SessionnResult, int32 index);
	
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* BTN_Refresh;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* ScrollBox;

	UPROPERTY(EditDefaultsOnly, category = "Menu Widget")
	TSubclassOf<UUserWidget> BrowserButtonAsset;
	
	UFUNCTION()
	void onRefresh();
};
