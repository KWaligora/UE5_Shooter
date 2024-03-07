#include "BSTAnimInstance.h"

#include "Characters/BSTCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UBSTAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	BSTCharacter = Cast<ABSTCharacter>(TryGetPawnOwner());
}

void UBSTAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (BSTCharacter == nullptr)
	{
		BSTCharacter = Cast<ABSTCharacter>(TryGetPawnOwner());
	}

	if (BSTCharacter == nullptr) return;

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
}