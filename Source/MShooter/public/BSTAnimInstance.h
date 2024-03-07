#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BSTAnimInstance.generated.h"

class ABSTCharacter;

UCLASS()
class MSHOOTER_API UBSTAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY(BlueprintReadOnly, Category="Character", meta=(AllowPrivateAccess = true))
	ABSTCharacter* BSTCharacter = nullptr;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess = true))
	float Speed = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess = true))
	bool bIsInAir = false;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess = true))
	bool bIsAccelerating = false;
};