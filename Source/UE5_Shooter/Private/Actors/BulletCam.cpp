#include "Actors/BulletCam.h"

#include "Actors/Projectiles/Projectile.h"
#include "Controllers/ShooterPlayerController.h"

// Sets default values
ABulletCam::ABulletCam()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
}

void ABulletCam::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (IsValid(World))
	{
		ShooterPlayerController = Cast<AShooterPlayerController>(World->GetFirstPlayerController());
	}
}

void ABulletCam::RunCamera(AProjectile* Projectile, FVector Direction)
{
	if (IsValid(Projectile))
	{
		OnRelease_DelegateHandle = Projectile->OnRelease.AddUObject(this, &ABulletCam::OnRelease);
		AttachToActor(Projectile, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		SetActorRotation(Direction.Rotation());
		ShooterPlayerController->SetViewTargetWithBlend(this);
	}
}

void ABulletCam::OnRelease(AProjectile* Projectile)
{
	if (IsValid(ShooterPlayerController))
	{		
		APawn* CurrentPawn = ShooterPlayerController->GetPawn();
		if (IsValid(CurrentPawn))
		{
			ShooterPlayerController->SetViewTargetWithBlend(CurrentPawn);
			
			Projectile->OnRelease.Remove(OnRelease_DelegateHandle);
		}
	}
}
