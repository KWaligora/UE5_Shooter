#include "SecurityVideoCamera.h"

ASecurityVideoCamera::ASecurityVideoCamera()
{
	PrimaryActorTick.bCanEverTick = true;

	CCTVBox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CCTVBox"));
	SetRootComponent(CCTVBox);
	
	CCTVSphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CCTVSphere"));
	CCTVSphere->SetupAttachment(RootComponent);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);	
}

void ASecurityVideoCamera::BeginPlay()
{
	Super::BeginPlay();
}

void ASecurityVideoCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

