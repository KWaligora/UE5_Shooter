// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UI/PlayerScoreWidget.h"
#include "UE5_ShooterGameMode.generated.h"

class AShooterHUD;

UCLASS(minimalapi)
class AUE5_ShooterGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	void AddPlayerPoints(int32 Score);
	UPlayerScoreWidget* GetPlayerScoreWidget();

	FORCEINLINE int32 GetPlayerPoints() {return PlayerPoints;}

protected:
	int32 PlayerPoints = 0;

	UPROPERTY()
	UPlayerScoreWidget* PlayerScoreWidget;
};