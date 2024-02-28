#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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

	int32 NumPublicConnections{4};
	FString MatchType{TEXT("FreeForAll")};
	
	TWeakObjectPtr<UMMCMultiplayerSessionSubsystem> MultiplayerSessionSubsystem;
	
public:
	virtual bool Initialize() override;
	virtual void NativeDestruct() override;
	
	UFUNCTION(BlueprintCallable)
	void MenuSetup(const int32 NumberOfPublicConnections = 4, const FString& TypeOfMatch = FString(TEXT("FreeForAll")));

private:
	UFUNCTION()
	void OnClickedJoinBtn();

	UFUNCTION()
	void OnClickedHostBtn();
	
	void MenuTearDown();
};
