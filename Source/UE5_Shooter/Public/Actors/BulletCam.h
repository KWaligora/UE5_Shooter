// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Actor.h"
#include "BulletCam.generated.h"

class AShooterPlayerController;
class AProjectile;

UCLASS()
class UE5_SHOOTER_API ABulletCam : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABulletCam();

	void RunCamera(AProjectile* Projectile);

	UFUNCTION()
	void OnRelease(AProjectile* Projectile);

protected:
	UPROPERTY(EditDefaultsOnly)
	FVector CameraOffset;
	
	UPROPERTY(VisibleDefaultsOnly)
	UCameraComponent* CameraComponent;

	UPROPERTY()
	AShooterPlayerController* ShooterPlayerController;

	virtual void BeginPlay() override;

private:
	FDelegateHandle OnRelease_DelegateHandle;
};
