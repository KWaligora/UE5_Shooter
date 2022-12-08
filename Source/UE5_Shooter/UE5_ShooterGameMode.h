// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShooterTypes.h"
#include "GameFramework/GameModeBase.h"
#include "UI/PlayerScoreWidget.h"
#include "UE5_ShooterGameMode.generated.h"

class AShooterHUD;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerPerceptionChange, EPlayerPerceptionState);

UCLASS(minimalapi)
class AUE5_ShooterGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	FOnPlayerPerceptionChange OnPlayerPerceptionChange;
	
	void AddPlayerPoints();
	UPlayerScoreWidget* GetPlayerScoreWidget();
	
	FORCEINLINE int32 GetPlayerPoints() {return PlayerPoints;}
	FORCEINLINE EPlayerPerceptionState GetPlayerPerceptionState() {return PlayerPerceptionState;}
	
	void TogglePerspective();

protected:
	int32 PlayerPoints = 0;

	EPlayerPerceptionState PlayerPerceptionState = EPlayerPerceptionState::TPS;

	UPROPERTY()
	UPlayerScoreWidget* PlayerScoreWidget;
};