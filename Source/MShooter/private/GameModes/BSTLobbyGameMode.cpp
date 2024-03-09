#include "GameModes/BSTLobbyGameMode.h"

#include "GameFramework/GameStateBase.h"

ABSTLobbyGameMode::ABSTLobbyGameMode()
{
	bUseSeamlessTravel = true;
}

void ABSTLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
	if (NumberOfPlayers < 2)
	{
		return;
	}

	UWorld* const World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	World->ServerTravel(FString("/Game/Maps/L_BST_Map?listen"));
}