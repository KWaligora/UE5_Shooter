#include "Actors/Projectiles/Projectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UE5_Shooter/UE5_ShooterGameMode.h"

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
	SphereCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
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

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (IsValid(World))
	{
		ShooterGameMode = Cast<AUE5_ShooterGameMode>(World->GetAuthGameMode()); 
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
	
	if (IsValid(ProjectileMovementComponent))
	{		
		ProjectileMovementComponent->Velocity = FireDirection * FireProjectileSpeed;
		ProjectileMovementComponent->UpdateComponentVelocity();
		if (IsValid(ProjectileMovementComponent->UpdatedPrimitive) && ProjectileMovementComponent->UpdatedPrimitive->IsSimulatingPhysics())
		{
			ProjectileMovementComponent->UpdatedPrimitive->SetPhysicsLinearVelocity(ProjectileMovementComponent->Velocity);
		}		
	}
	
	SetTimeToRelease();
}

void AProjectile::Destroyed()
{
	Super::Destroyed();

	UWorld* World = GetWorld();
	if (IsValid(World))
	{
		World->GetTimerManager().ClearTimer(TimeToRelease_TimerHandle);
		OnRelease.Clear();
	}
}

void AProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UWorld* World = GetWorld();
	if (IsValid(World))
	{
		ShooterGameMode = Cast<AUE5_ShooterGameMode>(World->GetAuthGameMode());
		if (IsValid(ShooterGameMode))
		{
			ShooterGameMode->OnPlayerPerceptionChange.AddUObject(this, &AProjectile::OnPerceptionChange);
		}
	}

	if (IsValid(SphereCollision))
	{
		SphereCollision->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	}

	SphereCollision->IgnoreActorWhenMoving(Owner, true);
}

void AProjectile::OnPerceptionChange(EPlayerPerceptionState PlayerPerceptionState)
{
	PlayerPerception = PlayerPerceptionState;
	if (PlayerPerception == FPS)
	{
		ProjectileMovementComponent->bShouldBounce = false;
	}
	else
	{
		ProjectileMovementComponent->bShouldBounce = true;
	}
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == Owner || OtherActor->IsA(StaticClass()))
	{
		return;
	}
	
	if (IsValid(ShooterGameMode) && ShooterGameMode->GetPlayerPerceptionState() == FPS)
	{
		Disable();
		if (OnRelease.IsBound())
		{
			OnRelease.Broadcast(this);
		}
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

void AProjectile::Disable()
{	
	if (IsValid(SphereCollision))
	{
		SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (IsValid(ProjectileMeshComponent))
	{
		ProjectileMeshComponent->SetVisibility(false);		
	}
}

