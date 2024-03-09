#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BSTLobbyGameMode.generated.h"

UCLASS()
class MSHOOTER_API ABSTLobbyGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ABSTLobbyGameMode();
	
	virtual void PostLogin(APlayerController* NewPlayer) override;
};