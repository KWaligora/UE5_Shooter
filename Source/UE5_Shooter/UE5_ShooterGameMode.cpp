// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE5_ShooterGameMode.h"

#include "UI/ShooterHUD.h"

void AUE5_ShooterGameMode::AddPlayerPoints(int32 Score)
{
	PlayerPoints += Score;

	PlayerScoreWidget = IsValid(PlayerScoreWidget) ? PlayerScoreWidget : GetPlayerScoreWidget();
	
	if (IsValid(PlayerScoreWidget))
	{
		PlayerScoreWidget->SetScore(PlayerPoints++);
	}
}

UPlayerScoreWidget* AUE5_ShooterGameMode::GetPlayerScoreWidget()
{
	UWorld* WorldInstance = GetWorld();
	if (IsValid(WorldInstance))
	{
		APlayerController* PlayerController = WorldInstance->GetFirstPlayerController();
		if (IsValid(PlayerController))
		{
			AShooterHUD* ShooterHUD = Cast<AShooterHUD>(PlayerController->GetHUD());
			return ShooterHUD->GetPlayerScoreWidget();			
		}
	}
	return nullptr;
}

void AUE5_ShooterGameMode::TogglePerspective()
{
	if(PlayerPerceptionState == TPS)
	{
		PlayerPerceptionState = FPS;
	}
	else
	{
		PlayerPerceptionState = TPS;
	}
}
