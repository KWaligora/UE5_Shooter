#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UE5_Shooter/UE5_ShooterGameMode.h"
#include "Projectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UBoxComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnRelease, class AProjectile*);

UCLASS()
class UE5_SHOOTER_API AProjectile : public AActor
{
	GENERATED_BODY()

public:
	/** Event broadcasted when TimeToRelease is up or hit other obj */
	FOnRelease OnRelease;
	
	AProjectile();

	virtual void BeginPlay() override;
	
	void SetTimeToRelease();

	void Fire(FVector Direction);
	
	void Enable();
	void Disable();

	virtual void Destroyed() override;

	virtual void PostInitializeComponents() override;

	FORCEINLINE float GetProjectileFireSpeed() {return FireProjectileSpeed;}
	FORCEINLINE const USphereComponent* GetSphereComponent() {return SphereCollision;}
	
protected:
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* ProjectileMeshComponent;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereCollision;

	UPROPERTY(EditDefaultsOnly)
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere)
	float FireProjectileSpeed = 3000.0f;

	UPROPERTY()
	AUE5_ShooterGameMode* ShooterGameMode;

	void OnPerceptionChange(EPlayerPerceptionState PlayerPerceptionState);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit);

private:
	float TimeToRelease = 2;
	FVector FireDirection = FVector(0.0f, 0.0f, 0.0f);
	
	FTimerHandle TimeToRelease_TimerHandle;

	EPlayerPerceptionState PlayerPerception;
};
