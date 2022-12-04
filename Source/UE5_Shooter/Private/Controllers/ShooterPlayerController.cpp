// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/ShooterPlayerController.h"

#include "GameFramework/GameModeBase.h"
#include "UE5_Shooter/UE5_ShooterCharacter.h"

void AShooterPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
	
	if (IsValid(WorldInstance))
	{
		WorldInstance->GetTimerManager().ClearTimer(Respawn_TimerHandle);
		WorldInstance->GetTimerManager().SetTimer(Respawn_TimerHandle, this, &ThisClass::Respawn, RespawnDelay);
	}
}

void AShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	WorldInstance = GetWorld();
	APawn* PlayerPawn = GetPawn();
	if (IsValid(PlayerPawn))
	{
		PlayerPawnClass = PlayerPawn->GetClass();
	}	
}

void AShooterPlayerController::Respawn()
{
	APawn* DefaultPawn = WorldInstance->SpawnActor<APawn>(PlayerPawnClass, SpawnLocation, FRotator::ZeroRotator);
	if (IsValid(DefaultPawn))
	{
		Possess(DefaultPawn);
	}
}
