// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerScoreWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class UE5_SHOOTER_API UPlayerScoreWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetScore(int32 Score);
	
protected:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* PlayerScoreText;
};
