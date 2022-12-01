#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ShooterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UE5_SHOOTER_API UShooterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
protected:
	UPROPERTY(BlueprintReadOnly, Category="Character")
	class AUE5_ShooterCharacter* ShooterCharacter;
	
	UPROPERTY(BlueprintReadOnly, Category="Movement")
	class UCharacterMovementComponent* CharacterMovementComponent; 

	UPROPERTY(BlueprintReadOnly, Category="Movement")
	float Speed = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category="Movement")
	float Direction = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category="Movement")
	bool bCanJump = true;

	UPROPERTY(BlueprintReadOnly, Category="Movement")
	bool bTryingToJump = false;

	UPROPERTY(BlueprintReadOnly, Category="Movement")
	bool bIsFalling = false;
};
