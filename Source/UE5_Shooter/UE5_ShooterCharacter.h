// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UE5_ShooterCharacter.generated.h"

class UCameraComponent;
class UShooterComponent;

UCLASS(config=Game)
class AUE5_ShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AUE5_ShooterCharacter();

	virtual void Tick(float DeltaSeconds) override;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input)
	float TurnRateGamepad;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return TPSCameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return TPSCamera; }

protected:	
	UPROPERTY(EditDefaultsOnly)
	UShooterComponent* ShooterComponent;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent* TPSCameraBoom;
	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* TPSCamera;

	UPROPERTY(VisibleAnywhere, Category="Camera")
	UCameraComponent* FPSCamera;
	
	UPROPERTY()
	class AUE5_ShooterGameMode* ShooterGameMode;

	virtual void BeginPlay() override;
	
	void MoveForward(float Value);
	
	void MoveRight(float Value);
	
	void TurnAtRate(float Rate);
	
	void LookUpAtRate(float Rate);
	AUE5_ShooterGameMode* GetGameMode();

	void SetupProjectilePrediction();
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ZoomIn();
	void ZoomOut();
	
	void Shoot();
	
	void TogglePerspective();
	
	void ActivateFPSCamera();
	void ActivateTPSCamera();
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};

