#include "SecurityVideoCamera.h"

ASecurityVideoCamera::ASecurityVideoCamera()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);
	
	CCTVBox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CCTVBox"));
	CCTVBox->SetupAttachment(RootComponent);
	
	CCTVSphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CCTVSphere"));
	CCTVSphere->SetupAttachment(RootComponent);	
}

void ASecurityVideoCamera::BeginPlay()
{
	Super::BeginPlay();
}

void ASecurityVideoCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

