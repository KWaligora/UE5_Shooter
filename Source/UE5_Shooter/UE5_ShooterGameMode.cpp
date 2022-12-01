// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE5_ShooterGameMode.h"
#include "UE5_ShooterCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUE5_ShooterGameMode::AUE5_ShooterGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
