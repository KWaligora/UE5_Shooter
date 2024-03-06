#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MMC_LobbyGameMode.generated.h"

UCLASS()
class MMC_MULTIPLAYERSESSION_API AMMC_LobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
};
