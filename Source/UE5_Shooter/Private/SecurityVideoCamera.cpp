#include "SecurityVideoCamera.h"
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

	if (IsValid(BoxComponent))
	{
		BoxComponent->OnComponentHit.AddDynamic(this, &ASecurityVideoCamera::OnHit);
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

void ASecurityVideoCamera::OnHit(UPrimitiveComponent* HitComponent,
                                 AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
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
