#include "BSTAnimInstance.h"

#include "Characters/BSTCharacter.h"
#include "Components/BSTCombatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void UBSTAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	BSTCharacter = Cast<ABSTCharacter>(TryGetPawnOwner());

	if (BSTCharacter == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UBSTAnimInstance::NativeInitializeAnimation - invalid BSTCharacter"))
		return;
	}

	UBSTCombatComponent* const CombatComponent = BSTCharacter->GetCombatComponent();
	if (CombatComponent == nullptr)
	{
		return;
	}

	CombatComponent->OnEquippedWeaponChanged.AddUObject(this, &UBSTAnimInstance::OnEquippedWeaponChanged);
	CombatComponent->OnAimChanged.AddUObject(this, &UBSTAnimInstance::OnAimChanged);
}

void UBSTAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (BSTCharacter == nullptr)
		return;

	FVector Velocity = BSTCharacter->GetVelocity();
	Velocity.Z = 0.0f;
	Speed = Velocity.Size();

	const UCharacterMovementComponent* const CharacterMovementComponent = BSTCharacter->GetCharacterMovement();
	if (!IsValid(CharacterMovementComponent))
	{
		return;
	}

	bIsInAir = CharacterMovementComponent->IsFalling();
	bIsAccelerating = CharacterMovementComponent->GetCurrentAcceleration().Size() > 0.0f;

	bIsCrouch = BSTCharacter->bIsCrouched;
}

void UBSTAnimInstance::OnEquippedWeaponChanged(ABSTWeapon* Weapon)
{
	bWeaponEquipped = Weapon != nullptr;
}

void UBSTAnimInstance::OnAimChanged(bool bAiming)
{
	bIsAiming = bAiming;
}