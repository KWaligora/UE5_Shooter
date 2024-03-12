#include "Weapons/BSTWeapon.h"

#include "Characters/BSTCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"

ABSTWeapon::ABSTWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetCollisionResponseToAllChannels(ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SetRootComponent(WeaponMesh);

	PickupSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	PickupSphere->SetupAttachment(RootComponent);
	PickupSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	PickupSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	PickupSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidget->SetupAttachment(RootComponent);
	PickupWidget->SetVisibility(false);
}

void ABSTWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABSTWeapon, WeaponState);
}

void ABSTWeapon::ShowPickupWidget(bool bShowWidget)
{
	if (!IsValid(PickupWidget))
	{
		return;
	}

	PickupWidget->SetVisibility(bShowWidget);
}

void ABSTWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		PickupSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		PickupSphere->OnComponentBeginOverlap.AddDynamic(this, &ABSTWeapon::OnPickupBeginOverlap);
		PickupSphere->OnComponentEndOverlap.AddDynamic(this, &ABSTWeapon::OnPickupEndOverlap);
	}
}

void ABSTWeapon::OnPickupBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                      const FHitResult& SweepResult)
{
	ABSTCharacter* const BSTCharacter = Cast<ABSTCharacter>(OtherActor);
	if (!IsValid(BSTCharacter))
	{
		return;
	}

	BSTCharacter->SetOverlappingWeapon_Sv(this);
}

void ABSTWeapon::OnPickupEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ABSTCharacter* const BSTCharacter = Cast<ABSTCharacter>(OtherActor);
	if (!IsValid(BSTCharacter))
	{
		return;
	}

	BSTCharacter->SetOverlappingWeapon_Sv(nullptr);
}

void ABSTWeapon::SetWeaponState(EBSTWeaponState NewWeaponState)
{
	if (!HasAuthority())
	{
		return;
	}

	if (!IsValid(PickupWidget) || !IsValid(PickupSphere))
	{
		return;
	}
	
	WeaponState = NewWeaponState;

	switch (NewWeaponState)
	{
		case EBSTWeaponState::Equipped:
		{
			PickupWidget->SetVisibility(false);
			PickupSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			break;
		}
		
		case EBSTWeaponState::Dropped:
		{
			PickupWidget->SetVisibility(true);
			PickupSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			break;
		}
		
		default: return;
	}
}

void ABSTWeapon::OnRep_WeaponState()
{
	if (!IsValid(PickupWidget))
	{
		return;
	}

	switch (WeaponState)
	{
		case EBSTWeaponState::Equipped: PickupWidget->SetVisibility(false);
			break;
		case EBSTWeaponState::Dropped: PickupWidget->SetVisibility(true);
			break;
		default: return;
	}
}