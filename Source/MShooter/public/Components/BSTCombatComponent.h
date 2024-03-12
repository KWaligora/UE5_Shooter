#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BSTCombatComponent.generated.h"

class ABSTCharacter;
class ABSTWeapon;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnEquippedWeaponChanged, ABSTWeapon* Weapon)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MSHOOTER_API UBSTCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FOnEquippedWeaponChanged OnEquippedWeaponChanged;

protected:
	TWeakObjectPtr<ABSTCharacter> BSTCharacterOwnerWeak;

	UPROPERTY(ReplicatedUsing=OnRep_EquippedWeaponWeak)
	TWeakObjectPtr<ABSTWeapon> EquippedWeaponWeak;

public:
	UBSTCombatComponent();

	virtual void InitializeComponent() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void EquipWeapon(ABSTWeapon* Weapon);

protected:
	UFUNCTION()
	void OnRep_EquippedWeaponWeak();
};