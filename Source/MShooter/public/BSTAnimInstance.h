#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Weapons/BSTWeapon.h"
#include "BSTAnimInstance.generated.h"

class ABSTCharacter;
class ABSTWeapon;

UCLASS()
class MSHOOTER_API UBSTAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

private:
	UPROPERTY(BlueprintReadOnly, Category="Character", meta=(AllowPrivateAccess = true))
	ABSTCharacter* BSTCharacter = nullptr;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess = true))
	float Speed = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess = true))
	bool bIsInAir = false;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess = true))
	bool bIsAccelerating = false;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess = true))
	bool bIsCrouch = false;

	UPROPERTY(BlueprintReadOnly, Category="Weapon", meta=(AllowPrivateAccess = true))
	bool bWeaponEquipped = false;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess = true))
	float YawOffset = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess = true))
	float Lean = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category="Weapon", meta=(AllowPrivateAccess = true))
	bool bIsAiming = false;

	FRotator CharacterRotationLastFrame = FRotator::ZeroRotator;
	FRotator CharacterRotation = FRotator::ZeroRotator;
	FRotator DeltaRotation = FRotator::ZeroRotator;

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	void OnEquippedWeaponChanged(ABSTWeapon* Weapon);
	void OnAimChanged(bool bAiming);
};