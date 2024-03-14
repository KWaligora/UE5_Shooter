#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Weapons/BSTWeapon.h"
#include "BSTCharacter.generated.h"

class UBSTCombatComponent;
class ABSTWeapon;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class MSHOOTER_API ABSTCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;

	UPROPERTY(ReplicatedUsing=OnRep_OverlappingWeapon)
	ABSTWeapon* OverlappingWeapon;

	UPROPERTY(VisibleAnywhere)
	UBSTCombatComponent* CombatComponent;

public:
	ABSTCharacter();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SetOverlappingWeapon_Sv(ABSTWeapon* Weapon);

	UBSTCombatComponent* GetCombatComponent() const { return CombatComponent; }

protected:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void CrouchBtnPressed();

	UFUNCTION(Server, Reliable)
	void AimBtnPressed();

	UFUNCTION(Server, Reliable)
	void AimBtnReleased();

	UFUNCTION(Server, Reliable)
	void ServerEquipBtnPressed();

	UFUNCTION()
	void OnRep_OverlappingWeapon(ABSTWeapon* LastWeapon);
};