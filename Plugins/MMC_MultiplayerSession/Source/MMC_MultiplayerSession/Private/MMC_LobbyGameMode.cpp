#include "MMC_LobbyGameMode.h"

#include "GameFramework/GameStateBase.h"

void AMMC_LobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (GameState == nullptr)
	{
		return;	
	}
	int32 NumberOfPlayers = GameState->PlayerArray.Num();
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 60.0f, FColor::Yellow, FString::Printf(TEXT("Players in game: %d"), NumberOfPlayers));
	}
}

void AMMC_LobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}