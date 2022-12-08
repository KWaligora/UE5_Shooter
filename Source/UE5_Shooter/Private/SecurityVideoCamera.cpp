#include "SecurityVideoCamera.h"

#include "Actors/Projectiles/Projectile.h"
#include "Components/BoxComponent.h"
#include "Components/ShooterComponent.h"
#include "UE5_Shooter/UE5_ShooterGameMode.h"

ASecurityVideoCamera::ASecurityVideoCamera()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetCollisionObjectType(ECC_Pawn);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	BoxComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	BoxComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	BoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	SetRootComponent(BoxComponent);
	
	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBoxComponent"));
	HitBox->SetupAttachment(RootComponent);

	// Should create ECC_Projectile but there is no time left ;C
	HitBox->SetCollisionObjectType(ECC_PhysicsBody);
	HitBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	HitBox->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	
	CCTVBox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CCTVBox"));
	CCTVBox->SetupAttachment(RootComponent);
	
	CCTVSphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CCTVSphere"));
	CCTVSphere->SetupAttachment(RootComponent);

	ShooterComponent = CreateDefaultSubobject<UShooterComponent>(TEXT("ShooterComponent"));

	bUseControllerRotationYaw = true;
}

void ASecurityVideoCamera::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (IsValid(HitBox))
	{
		HitBox->OnComponentBeginOverlap.AddDynamic(this, &ASecurityVideoCamera::OnBeginOverlap);
	}	
}

void ASecurityVideoCamera::BeginPlay()
{
	Super::BeginPlay();
	
	UWorld* WorldInstance = GetWorld();
	if (IsValid(WorldInstance))
	{
		ShooterGameMode = Cast<AUE5_ShooterGameMode>(WorldInstance->GetAuthGameMode());
	}
}

void ASecurityVideoCamera::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	AProjectile* Projectile = Cast<AProjectile>(OtherActor);
	if (IsValid(Projectile))
	{
		if (IsValid(ShooterGameMode))
		{
			ShooterGameMode->AddPlayerPoints();
		}
	}
}
