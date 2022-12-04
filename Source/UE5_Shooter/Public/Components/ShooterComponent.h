// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Projectiles/Projectile.h"
#include "Components/ActorComponent.h"
#include "ShooterComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UShooterComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UShooterComponent();
	
	void PlayerShoot();

	void AIShoot(AActor* Target);

protected:
	UPROPERTY()
	AActor* Owner;

	UPROPERTY(EditDefaultsOnly)
	float ProjectileSpawnOffset = 10.0f;

	virtual void BeginPlay() override;
	
	AProjectile* GetProjectile();

private:
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = "true"))
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY()
	TArray<AProjectile*> ProjectilePool;
	
	/** Prevent from being garbage collected */
	UPROPERTY()
	TArray<AProjectile*> AllProjectiles;

	void TraceUnderCrosshair(FHitResult& HitResult);
	
	AProjectile* CreateNewProjectile();
	void CalculatePlayerFireDirection(FVector& FireDirection);

	UFUNCTION()
	void OnReleaseToPool(AProjectile* Projectile);	
};
