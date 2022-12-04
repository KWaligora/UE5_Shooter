#include "SecurityVideoCamera.h"
#include "Components/BoxComponent.h"
#include "Components/ShooterComponent.h"

ASecurityVideoCamera::ASecurityVideoCamera()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);
	
	CCTVBox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CCTVBox"));
	CCTVBox->SetupAttachment(RootComponent);
	
	CCTVSphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CCTVSphere"));
	CCTVSphere->SetupAttachment(RootComponent);

	ShooterComponent = CreateDefaultSubobject<UShooterComponent>(TEXT("ShooterComponent"));

	bUseControllerRotationYaw = true;
}

