#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BSTCombatComponent.generated.h"

class ABSTCharacter;
class ABSTWeapon;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MSHOOTER_API UBSTCombatComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	UBSTCombatComponent();
	
	virtual void InitializeComponent() override;

	void EquipWeapon(ABSTWeapon* Weapon);
	
protected:
	TWeakObjectPtr<ABSTCharacter> BSTCharacterOwnerWeak;
	TWeakObjectPtr<ABSTWeapon> EquippedWeaponWeak;
};