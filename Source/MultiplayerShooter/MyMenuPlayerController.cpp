// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMenuPlayerController.h"
#include "MyServerBrowser.h"

void AMyMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (BrowserWidget) {
		BrowserInst = CreateWidget<UMyServerBrowser>(GetWorld(), BrowserWidget);
		BrowserInst->AddToViewport();
	}
}
