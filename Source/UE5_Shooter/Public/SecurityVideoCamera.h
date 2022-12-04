// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "SecurityVideoCamera.generated.h"

class AUE5_ShooterGameMode;
class UShooterComponent;

UCLASS()
class ASecurityVideoCamera : public APawn
{
	GENERATED_BODY()

public:
	ASecurityVideoCamera();
	virtual void PostInitializeComponents() override;

	FORCEINLINE UShooterComponent* GetShooterComponent() {return ShooterComponent;}

protected:
	UPROPERTY(VisibleAnywhere)
	UShooterComponent* ShooterComponent;

	UPROPERTY()
	AUE5_ShooterGameMode* ShooterGameMode;

	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit );
	
private:
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* CCTVBox;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* CCTVSphere;

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* BoxComponent;
};
