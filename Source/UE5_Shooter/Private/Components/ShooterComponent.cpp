#include "Components/ShooterComponent.h"

#include "Actors/BulletCam.h"
#include "Actors/Projectiles/Projectile.h"
#include "Kismet/GameplayStatics.h"

UShooterComponent::UShooterComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UShooterComponent::SpawnBulletCam()
{
	if (IsValid(BulletCamClass))
	{
		UWorld* World = GetWorld();
		if(IsValid(World))
		{
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.Owner = Owner;
			BulletCam = World->SpawnActor<ABulletCam>(BulletCamClass, SpawnParameters);
		}	
	}
}

void UShooterComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
	ensure(Owner != nullptr);

	if (bBulletCameEnabled)
	{
		SpawnBulletCam();		
	}

	UWorld* World = GetWorld();
	if (IsValid(World))
	{
		ShooterGameMode = Cast<AUE5_ShooterGameMode>(World->GetAuthGameMode());
	}
}

AProjectile* UShooterComponent::GetProjectile()
{
	// Try to get from pool
	if (!ProjectilePool.IsEmpty() && IsValid(Owner))
	{
		AProjectile* Projectile = ProjectilePool.Pop();
		if (IsValid(Projectile))
		{			
			Projectile->Enable();
			return Projectile;
		}		
	}

	// It pool is empty try to create one
	return CreateNewProjectile();
}

void UShooterComponent::TraceUnderCrosshair(FHitResult& HitResult)
{
FVector2d ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	FVector2d CrosshairLocation(ViewportSize.X / 2.0f, ViewportSize.Y / 2.0f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;
	
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection
		);

	if (bScreenToWorld)
	{
		FVector Start = CrosshairWorldPosition;

		if (IsValid(Owner))
		{
			float DistanceToCharacter = (Owner->GetActorLocation() - Start).Size();
			Start += CrosshairWorldDirection * (DistanceToCharacter + 100.0f);
		}
		
		FVector End = Start + CrosshairWorldDirection * 8000.0f;

		GetWorld()->LineTraceSingleByChannel(
			HitResult,
			Start,
			End,
			ECC_Visibility
			);
	}
}

AProjectile* UShooterComponent::CreateNewProjectile()
{
	UWorld* World = GetWorld();
	if (IsValid(World))
	{
		if (IsValid(Owner))
		{
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.Owner = Owner;

			FVector StartPoint = Owner->GetActorLocation() + Owner->GetActorForwardVector() * ProjectileSpawnOffset;
			AProjectile* Projectile = World->SpawnActor<AProjectile>(ProjectileClass, StartPoint, FRotator::ZeroRotator, SpawnParameters);
			if (IsValid(Projectile))
			{
				Projectile->OnRelease.AddUObject(this, &UShooterComponent::OnReleaseToPool);
				AllProjectiles.Add(Projectile);
				return Projectile;
			}			
		}
	}
	UE_LOG(LogTemp, Error, TEXT("Failed to create new projectile!"));
	return nullptr;
}

void UShooterComponent::PlayerShoot()
{
	AProjectile* Projectile = GetProjectile();	
	if (IsValid(Projectile) && IsValid(Owner))
	{
		FVector FireDirection;
		CalculatePlayerFireDirection(FireDirection);

		FVector StartPoint = Owner->GetActorLocation() + Owner->GetActorForwardVector() * ProjectileSpawnOffset;
		Projectile->SetActorLocation(StartPoint);
		
		Projectile->Fire(FireDirection);
		
		if (bBulletCameEnabled && ShooterGameMode->GetPlayerPerceptionState() == FPS && IsValid(BulletCam))
		{
			BulletCam->RunCamera(Projectile);
		}
	}
}

void UShooterComponent::CalculatePlayerFireDirection(FVector& FireDirection)
{
	if (IsValid(Owner))
	{
		FHitResult HitResult;
		TraceUnderCrosshair(HitResult);

		FVector StartPoint = Owner->GetActorLocation() + Owner->GetActorForwardVector() * ProjectileSpawnOffset;
		FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : HitResult.TraceEnd; 
		
		FireDirection = (EndPoint - StartPoint).GetSafeNormal();
	}
}

void UShooterComponent::AIShoot(AActor* Target)
{
	AProjectile* Projectile = GetProjectile();
	if (IsValid(Projectile))
	{
		FVector StartPoint = Owner->GetActorLocation() + Owner->GetActorForwardVector() * ProjectileSpawnOffset;
		Projectile->SetActorLocation(StartPoint);

		if (IsValid(Target))
		{
			FVector FireDirection = (Target->GetActorLocation() - StartPoint).GetSafeNormal();
			Projectile->Fire(FireDirection);
		}		
	}
}

void UShooterComponent::OnReleaseToPool(AProjectile* Projectile)
{
	Projectile->Disable();	
	ProjectilePool.Add(Projectile);
}
