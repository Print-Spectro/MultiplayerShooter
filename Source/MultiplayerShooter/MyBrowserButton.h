// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyBrowserButton.generated.h"

/**
 *hi 
 */

class UTextBlock;
class UButton;
class FOnlineSessionSearchResult;

/**
 * 
 */
UCLASS()
class MULTIPLAYERSHOOTER_API UMyBrowserButton : public UUserWidget
{
	GENERATED_BODY()
		virtual void NativeConstruct() override;

protected:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* TXT_OwningPlayer;


	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* TXT_Occupancy;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* BTN_Server;

	UPROPERTY()
	int32 SessionToJoin;

public:

	UFUNCTION(BlueprintCallable)
		void setupButton(const FString Occupancy, const FString OwningPlayer, const int32 SessionIndex);

	UFUNCTION()
		void onButtonPress();


};
