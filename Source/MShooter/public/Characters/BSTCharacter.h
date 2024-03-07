#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BSTCharacter.generated.h"

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

public:
	ABSTCharacter();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
protected:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
};