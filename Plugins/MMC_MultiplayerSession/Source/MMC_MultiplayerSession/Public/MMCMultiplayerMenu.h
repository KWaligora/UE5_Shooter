#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MMCMultiplayerMenu.generated.h"

class UMMCMultiplayerSessionSubsystem;
class UButton;

UCLASS()
class MMC_MULTIPLAYERSESSION_API UMMCMultiplayerMenu : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta=(BindWidget))
	UButton* JoinBtn;

	UPROPERTY(meta=(BindWidget))
	UButton* HostBtn;

	UPROPERTY(meta=(BindWidget))
	UButton* QuitBtn;
	
	int32 NumPublicConnections{4};
	FString MatchType{TEXT("FreeForAll")};

	FString PathToLobby{TEXT("")};

	TWeakObjectPtr<UMMCMultiplayerSessionSubsystem> MultiplayerSessionSubsystem;

public:
	virtual bool Initialize() override;
	virtual void NativeDestruct() override;

	UFUNCTION(BlueprintCallable)
	void MenuSetup(const int32 NumberOfPublicConnections = 4, const FString& TypeOfMatch = FString(TEXT("FreeForAll")), const FString& LobbyPath = FString(TEXT("/Game/Maps/L_Lobby")));

private:
	UFUNCTION()
	void OnClickedJoinBtn();

	UFUNCTION()
	void OnClickedHostBtn();

	UFUNCTION()
	void OnSessionCreated(bool bWasSuccessful);

	void OnFoundSession(const TArray<FOnlineSessionSearchResult>& OnlineSessionSearchResults, bool bWasSuccessful);

	void OnJoinedSession(EOnJoinSessionCompleteResult::Type Result);

	UFUNCTION()
	void OnSessionStarted(bool bWasSuccessful);

	UFUNCTION()
	void OnSessionDestroyed(bool bWasSuccessful);

	void MenuTearDown();
};