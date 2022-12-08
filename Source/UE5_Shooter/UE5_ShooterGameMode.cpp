// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE5_ShooterGameMode.h"

#include "UI/ShooterHUD.h"

void AUE5_ShooterGameMode::AddPlayerPoints()
{
	if (PlayerPerceptionState == EPlayerPerceptionState::FPS)
	{
		PlayerPoints += 5;
	}
	else
	{
		PlayerPoints += 1;
	}

	PlayerScoreWidget = IsValid(PlayerScoreWidget) ? PlayerScoreWidget : GetPlayerScoreWidget();
	
	if (IsValid(PlayerScoreWidget))
	{
		PlayerScoreWidget->SetScore(PlayerPoints);
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
	if(PlayerPerceptionState == EPlayerPerceptionState::TPS)
	{
		PlayerPerceptionState = EPlayerPerceptionState::FPS;
	}
	else
	{
		PlayerPerceptionState = EPlayerPerceptionState::TPS;
	}

	if (OnPlayerPerceptionChange.IsBound())
	{
		OnPlayerPerceptionChange.Broadcast(PlayerPerceptionState);
	}
}
