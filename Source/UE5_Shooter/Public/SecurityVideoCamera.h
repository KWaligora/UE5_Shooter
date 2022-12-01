// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Pawn.h"
#include "SecurityVideoCamera.generated.h"

UCLASS()
class UE5_SHOOTER_API ASecurityVideoCamera : public APawn
{
	GENERATED_BODY()

public:
	ASecurityVideoCamera();
	
	virtual void Tick(float DeltaTime) override;    

protected:	
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* CCTVBox;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* CCTVSphere;

	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* BoxComponent;
};
