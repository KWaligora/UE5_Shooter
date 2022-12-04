// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ShooterHUD.generated.h"

class UPlayerScoreWidget;

/**
 * 
 */
UCLASS()
class UE5_SHOOTER_API AShooterHUD : public AHUD
{
	GENERATED_BODY()

public:
	FORCEINLINE UPlayerScoreWidget* GetPlayerScoreWidget() {return PlayerScoreWidget;}
	
protected:	
	virtual void BeginPlay() override;

	UPROPERTY()
	UPlayerScoreWidget* PlayerScoreWidget;

private:
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess))
	TSubclassOf<UUserWidget> CrosshairWidgetClass;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess))
	TSubclassOf<UPlayerScoreWidget> PlayerScoreWidgetClass;
};
