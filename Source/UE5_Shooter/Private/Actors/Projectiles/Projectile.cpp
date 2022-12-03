#include "Actors/Projectiles/Projectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetCollisionObjectType(ECC_WorldDynamic);
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SphereCollision->SetCollisionResponseToAllChannels(ECR_Ignore);	
	SphereCollision->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	SphereCollision->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	SphereCollision->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	SphereCollision->SetSimulatePhysics(true);
	SetRootComponent(SphereCollision);

	ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	ProjectileMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProjectileMeshComponent->SetupAttachment(RootComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->InitialSpeed = 0.0f;
	ProjectileMovementComponent->MaxSpeed = 20000.0f;
}

void AProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (IsValid(SphereCollision))
	{		
		SphereCollision->OnComponentHit.AddDynamic(this, &AProjectile::OnProjectileHit);
		if (IsValid(Owner))
		{
			SphereCollision->IgnoreActorWhenMoving(Owner, true);
		}		
	}	
}

void AProjectile::SetTimeToRelease()
{
	UWorld* World = GetWorld();
	if (IsValid(World))
	{		
		FTimerManager& TimerManager = World->GetTimerManager();
		TimerManager.ClearTimer(TimeToRelease_TimerHandle);

		FTimerDelegate TimerDelegate;
		TimerDelegate.BindLambda([&]
		{
			if (OnRelease.IsBound())
			{
				OnRelease.Broadcast(this);
			}
		});
		
		TimerManager.SetTimer(TimeToRelease_TimerHandle, TimerDelegate, TimeToRelease, false);
	}	
}

void AProjectile::Fire(FVector Direction)
{
	FireDirection = Direction;
	
	// Tick uses ProjectileMovementComponent so it must be valid
	if (IsValid(ProjectileMovementComponent))
	{
		ProjectileMovementComponent->Velocity = FireDirection * FireProjectileSpeed;
		ProjectileMovementComponent->UpdateComponentVelocity();
		ProjectileMovementComponent->UpdatedPrimitive->SetPhysicsLinearVelocity(ProjectileMovementComponent->Velocity);
	}
	
	SetTimeToRelease();
}

void AProjectile::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OnRelease.IsBound())
	{
		OnRelease.Broadcast(this);		
	}

	UWorld* World = GetWorld();
	if (IsValid(World))
	{
		World->GetTimerManager().ClearTimer(TimeToRelease_TimerHandle);
	}
}

void AProjectile::Disable()
{
	PrimaryActorTick.SetTickFunctionEnable(false);
	
	if (IsValid(SphereCollision))
	{
		SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (IsValid(ProjectileMeshComponent))
	{
		ProjectileMeshComponent->SetVisibility(false);
	}
}

void AProjectile::Enable()
{
	if (IsValid(SphereCollision))
	{
		SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}

	if (IsValid(ProjectileMeshComponent))
	{
		ProjectileMeshComponent->SetVisibility(true);
	}
}

