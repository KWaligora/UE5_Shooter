#pragma once

#include "CoreMinimal.h"
#include "MShooter.h"
#include "Components/ActorComponent.h"
#include "BSTCombatComponent.generated.h"

class UCharacterMovementComponent;

class ABSTCharacter;
class ABSTWeapon;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnEquippedWeaponChanged, ABSTWeapon* Weapon)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAimChanged, bool bAiming)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MSHOOTER_API UBSTCombatComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	FOnEquippedWeaponChanged OnEquippedWeaponChanged;
	FOnAimChanged OnAimChanged;
	
protected:
	TWeakObjectPtr<ABSTCharacter> BSTCharacterOwnerWeak;
	TWeakObjectPtr<UCharacterMovementComponent> CharacterMovementComponent;

	UPROPERTY(ReplicatedUsing=OnRep_EquippedWeaponWeak)
	TWeakObjectPtr<ABSTWeapon> EquippedWeaponWeak;

	UPROPERTY(ReplicatedUsing=OnRep_bAiming)
	bool bAiming = false;

	UPROPERTY(EditAnywhere)
	float BaseWalkSpeed = 600.0f;

	UPROPERTY(EditAnywhere)
	float AimWalkSpeed = 400.0f;
	
public:
	UBSTCombatComponent();

	virtual void InitializeComponent() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void EquipWeapon(ABSTWeapon* Weapon);
	void SetIsAiming(bool bIsAiming);

protected:
	UFUNCTION()
	void OnRep_EquippedWeaponWeak();

	UFUNCTION()
	void OnRep_bAiming();
};