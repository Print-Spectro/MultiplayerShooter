// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyMenuPlayerController.generated.h"

/**
 * 
 */

 class UMyServerBrowser;
UCLASS()
class MULTIPLAYERSHOOTER_API AMyMenuPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	virtual void BeginPlay() override;


	UPROPERTY(EditDefaultsOnly, category = "Menu Widget")
	TSubclassOf<UUserWidget> BrowserWidget;

public:
	UPROPERTY()
	UMyServerBrowser* BrowserInst;

};
