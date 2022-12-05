// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/GameplayStaticsTypes.h"
#include "ShooterComponent.generated.h"

class AUE5_ShooterGameMode;
class AProjectile;
class ABulletCam;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UShooterComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UShooterComponent();
	void SpawnBulletCam();

	void PlayerShoot();

	void AIShoot(AActor* Target);

	void GetPredictProjectilePathParams(FPredictProjectilePathParams& PredictProjectilePathParams);
	
protected:
	UPROPERTY()
	AActor* Owner;

	UPROPERTY(EditDefaultsOnly)
	float ProjectileSpawnOffset = 10.0f;

	UPROPERTY()
	ABulletCam* BulletCam;

	virtual void BeginPlay() override;
	
	AProjectile* GetProjectile();

private:
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = "true"))
	TSubclassOf<AProjectile> ProjectileClass;
	
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = "true"))
	TSubclassOf<ABulletCam> BulletCamClass;

	UPROPERTY()
	TArray<AProjectile*> ProjectilePool;
	
	/** Prevent from being garbage collected */
	UPROPERTY()
	TArray<AProjectile*> AllProjectiles;

	UPROPERTY()
	AUE5_ShooterGameMode* ShooterGameMode;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = "true") )
	bool bBulletCameEnabled = false;
	
	void TraceUnderCrosshair(FHitResult& HitResult);
	
	AProjectile* CreateNewProjectile();
	void CalculatePlayerFireDirection(FVector& FireDirection);

	UFUNCTION()
	void OnReleaseToPool(AProjectile* Projectile);	
	
	float ProjectileSpeed = 0;
	float ProjectileSphereRadius = 0;
};
