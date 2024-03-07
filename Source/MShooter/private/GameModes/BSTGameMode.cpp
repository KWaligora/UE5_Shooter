#include "GameModes/BSTGameMode.h"

#include "Characters/BSTCharacter.h"

ABSTGameMode::ABSTGameMode()
{
	DefaultPawnClass = ABSTCharacter::StaticClass();
}