#include "SecurityVideoCamera.h"
#include "Components/BoxComponent.h"

ASecurityVideoCamera::ASecurityVideoCamera()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);
	
	CCTVBox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CCTVBox"));
	CCTVBox->SetupAttachment(RootComponent);
	
	CCTVSphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CCTVSphere"));
	CCTVSphere->SetupAttachment(RootComponent);

	bUseControllerRotationYaw = true;
}

void ASecurityVideoCamera::BeginPlay()
{
	Super::BeginPlay();
}

void ASecurityVideoCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

