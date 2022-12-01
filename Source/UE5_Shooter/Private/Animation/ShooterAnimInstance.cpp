#include "Animation/ShooterAnimInstance.h"

#include "KismetAnimationLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UE5_Shooter/UE5_ShooterCharacter.h"

void UShooterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	ShooterCharacter = Cast<AUE5_ShooterCharacter>(TryGetPawnOwner());
	if (!IsValid(ShooterCharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("ShooterCharacter is not valid in UShooterAnimInstance"));
		return;
	}
	
	CharacterMovementComponent = ShooterCharacter->GetCharacterMovement();
	if (!IsValid(CharacterMovementComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("CharacterMovementComponent is not valid in UShooterAnimInstance"));		
	}	
}

void UShooterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!IsValid(ShooterCharacter) || !IsValid(CharacterMovementComponent))
	{
		return;
	}

	FVector OwnerVelocity = ShooterCharacter->GetVelocity();
	
	Speed = OwnerVelocity.Length();
	Direction = UKismetAnimationLibrary::CalculateDirection(OwnerVelocity, ShooterCharacter->GetActorRotation());
	bCanJump = ShooterCharacter->CanJump();
	bTryingToJump = ShooterCharacter->bPressedJump;
	bIsFalling = CharacterMovementComponent->IsFalling();
}
