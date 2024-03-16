#include "BSTAnimInstance.h"

#include "Characters/BSTCharacter.h"
#include "Components/BSTCombatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

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

	FRotator AimRotation = BSTCharacter->GetBaseAimRotation();
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(BSTCharacter->GetVelocity());;
	FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);
	DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaRot, DeltaSeconds, 6.0f);
	YawOffset = DeltaRotation.Yaw;
	
	CharacterRotationLastFrame = CharacterRotation;
	CharacterRotation = BSTCharacter->GetActorRotation();
	const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);
	const float Target  = Delta.Yaw / DeltaSeconds;
	const float Interp = FMath::FInterpTo(Lean, Target, DeltaSeconds, 6.0f);
	Lean = FMath::Clamp(Interp, -90.0f, 90.0f);
}

void UBSTAnimInstance::OnEquippedWeaponChanged(ABSTWeapon* Weapon)
{
	bWeaponEquipped = Weapon != nullptr;
}

void UBSTAnimInstance::OnAimChanged(bool bAiming)
{
	bIsAiming = bAiming;
}