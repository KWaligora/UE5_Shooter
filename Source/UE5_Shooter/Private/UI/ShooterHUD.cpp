// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ShooterHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/PlayerScoreWidget.h"

void AShooterHUD::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = GetOwningPlayerController();
	if (IsValid(PlayerController))
	{
		if (IsValid(CrosshairWidgetClass))
		{
			UUserWidget* CrosshairWidget = CreateWidget<UUserWidget>(PlayerController, CrosshairWidgetClass);
			if (IsValid(CrosshairWidget))
			{
				CrosshairWidget->AddToViewport();
			}			
		}
		
		if (IsValid(PlayerScoreWidgetClass))
		{
			PlayerScoreWidget = CreateWidget<UPlayerScoreWidget>(PlayerController, PlayerScoreWidgetClass);
			if (IsValid(PlayerScoreWidget))
			{
				PlayerScoreWidget->AddToViewport();	
			}			
		}		
	}
}
