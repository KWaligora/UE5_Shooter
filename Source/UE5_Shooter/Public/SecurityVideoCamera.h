// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "SecurityVideoCamera.generated.h"

class UShooterComponent;
UCLASS()
class ASecurityVideoCamera : public APawn
{
	GENERATED_BODY()

public:
	ASecurityVideoCamera();

	FORCEINLINE UShooterComponent* GetShooterComponent() {return ShooterComponent;}

protected:
	UPROPERTY(VisibleAnywhere)
	UShooterComponent* ShooterComponent;	
	
private:
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* CCTVBox;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* CCTVSphere;

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* BoxComponent;
};
