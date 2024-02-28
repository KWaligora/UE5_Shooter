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