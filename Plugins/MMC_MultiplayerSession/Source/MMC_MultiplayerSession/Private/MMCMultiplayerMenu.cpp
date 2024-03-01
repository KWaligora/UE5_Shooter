#include "MMCMultiplayerMenu.h"

#include "MMCMultiplayerSessionSubsystem.h"
#include "Components/Button.h"

void UMMCMultiplayerMenu::MenuSetup(const int32 NumberOfPublicConnections /* = 4*/, const FString& TypeOfMatch /* = "FreeForAll"*/)
{
	NumPublicConnections = NumberOfPublicConnections;
	MatchType = TypeOfMatch;

	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	SetIsFocusable(true);

	const UWorld* const World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	APlayerController* const PlayerController = World->GetFirstPlayerController();
	if (!IsValid(PlayerController))
	{
		return;
	}

	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(InputModeData);
	PlayerController->SetShowMouseCursor(true);

	const UGameInstance* const GameInstance = GetGameInstance();
	if (GameInstance == nullptr)
	{
		return;
	}

	MultiplayerSessionSubsystem = GameInstance->GetSubsystem<UMMCMultiplayerSessionSubsystem>();
	if (MultiplayerSessionSubsystem == nullptr)
	{
		return;
	}

	MultiplayerSessionSubsystem->OnMultiplayerCreateSessionComplete.AddDynamic(this, &UMMCMultiplayerMenu::OnSessionCreated);
	MultiplayerSessionSubsystem->OnMultiplayerDestroySessionComplete.AddDynamic(this, &UMMCMultiplayerMenu::OnSessionDestroyed);
	MultiplayerSessionSubsystem->OnMultiplayerStartSessionComplete.AddDynamic(this, &UMMCMultiplayerMenu::OnSessionStarted);
	
	MultiplayerSessionSubsystem->OnMultiplayerFindSessionsComplete.AddUObject(this, &UMMCMultiplayerMenu::OnFoundSession);
	MultiplayerSessionSubsystem->OnMultiplayerJoinSessionComplete.AddUObject(this, &UMMCMultiplayerMenu::OnJoinedSession);
}

bool UMMCMultiplayerMenu::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (!IsValid(JoinBtn) || !IsValid(HostBtn))
	{
		return false;
	}

	JoinBtn->OnClicked.AddDynamic(this, &UMMCMultiplayerMenu::OnClickedJoinBtn);
	HostBtn->OnClicked.AddDynamic(this, &UMMCMultiplayerMenu::OnClickedHostBtn);

	return true;
}

void UMMCMultiplayerMenu::NativeDestruct()
{
	MenuTearDown();

	Super::NativeDestruct();
}

void UMMCMultiplayerMenu::OnClickedJoinBtn()
{
	if (!MultiplayerSessionSubsystem.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("UMMCMultiplayerMenu::OnClickedHostBtn - invalid MultiplayerSessionSubsystem"))
		return;
	}

	MultiplayerSessionSubsystem->FindSessions(10000);
}

void UMMCMultiplayerMenu::OnClickedHostBtn()
{
	if (!MultiplayerSessionSubsystem.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("UMMCMultiplayerMenu::OnClickedHostBtn - invalid MultiplayerSessionSubsystem"))
		return;
	}

	MultiplayerSessionSubsystem->CreateSession(NumPublicConnections, MatchType);

	UWorld* const World = GetWorld();
	if (IsValid(World))
	{
		World->ServerTravel("/Game/Maps/L_Lobby?listen", true);
	}
}

void UMMCMultiplayerMenu::OnSessionCreated(bool bWasSuccessful)
{
	if (GEngine && bWasSuccessful)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Magenta, TEXT("SessionCreated"));
	}
}

void UMMCMultiplayerMenu::OnFoundSession(const TArray<FOnlineSessionSearchResult>& OnlineSessionSearchResults, bool bWasSuccessful)
{
	if (MultiplayerSessionSubsystem == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UMMCMultiplayerMenu::OnFoundSession - invalid MultiplayerSessionSubsystem"))
		return;
	}
		
	for (FOnlineSessionSearchResult Result : OnlineSessionSearchResults)
	{
		FString MatchTypeValue;
		Result.Session.SessionSettings.Get(FName("MatchType"), MatchTypeValue);

		if (MatchTypeValue == MatchType)
		{
			MultiplayerSessionSubsystem->JoinSession(Result);
			return;
		}
	}
}

void UMMCMultiplayerMenu::OnJoinedSession(EOnJoinSessionCompleteResult::Type Result)
{
	if (Result != EOnJoinSessionCompleteResult::Type::Success)
	{
		return;
	}
	
	IOnlineSubsystem* const OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UMMCMultiplayerMenu::OnJoinedSession - invalid OnlineSubsystem"))
		return;
	}

	const IOnlineSessionPtr OnlineSession = OnlineSubsystem->GetSessionInterface();
	if (!OnlineSession.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("UMMCMultiplayerMenu::OnJoinedSession - invalid OnlineSession"))
		return;
	}

	const UGameInstance* const GameInstance = GetGameInstance();
	if (GameInstance == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UMMCMultiplayerMenu::OnJoinedSession - invalid GameInstance"))
		return;
	}

	APlayerController* const PlayerController = GameInstance->GetFirstLocalPlayerController();
	if (!IsValid(PlayerController))
	{
		UE_LOG(LogTemp, Error, TEXT("UMMCMultiplayerMenu::OnJoinedSession - invalid PlayerController"))
		return;
	}

	FString Address;
	if (OnlineSession->GetResolvedConnectString(NAME_GameSession, Address))
	{
		PlayerController->ClientTravel(Address, TRAVEL_Absolute);
	}	
}

void UMMCMultiplayerMenu::OnSessionStarted(bool bWasSuccessful)
{
}

void UMMCMultiplayerMenu::OnSessionDestroyed(bool bWasSuccessful)
{
}

void UMMCMultiplayerMenu::MenuTearDown()
{
	RemoveFromParent();

	const UWorld* const World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}

	APlayerController* const PlayerController = World->GetFirstPlayerController();
	if (!IsValid(PlayerController))
	{
		return;
	}

	PlayerController->SetInputMode(FInputModeGameOnly());
	PlayerController->SetShowMouseCursor(false);
}