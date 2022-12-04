// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerScoreWidget.h"

#include "Components/TextBlock.h"

void UPlayerScoreWidget::SetScore(int32 Score)
{
	if (IsValid(PlayerScoreText))
	{
		PlayerScoreText->SetText(FText::AsNumber(Score));
	}
}
