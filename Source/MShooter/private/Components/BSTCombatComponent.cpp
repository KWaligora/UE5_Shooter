#include "Components/BSTCombatComponent.h"

#include "Weapons/BSTWeapon.h"
#include "Characters/BSTCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

UBSTCombatComponent::UBSTCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	bWantsInitializeComponent = true;
}

void UBSTCombatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	BSTCharacterOwnerWeak = GetOwner<ABSTCharacter>();
}

void UBSTCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UBSTCombatComponent, EquippedWeaponWeak);
	DOREPLIFETIME(UBSTCombatComponent, bAiming);
}

void UBSTCombatComponent::EquipWeapon(ABSTWeapon* Weapon)
{
	if (!BSTCharacterOwnerWeak.IsValid() || !IsValid(Weapon))
	{
		BSTCharacterOwnerWeak->GetCharacterMovement()->bOrientRotationToMovement = true;
		BSTCharacterOwnerWeak->bUseControllerRotationYaw = false;
		return;
	}

	EquippedWeaponWeak = Weapon;

	Weapon->SetOwner(BSTCharacterOwnerWeak.Get());
	Weapon->SetWeaponState(EBSTWeaponState::Equipped);

	USkeletalMeshComponent* const SkeletalMeshComponent = BSTCharacterOwnerWeak->GetMesh();
	if (SkeletalMeshComponent == nullptr)
	{
		return;
	}

	const USkeletalMeshSocket* SkeletalMeshSocket = SkeletalMeshComponent->GetSocketByName(FName("RightHandSocket"));
	if (SkeletalMeshSocket == nullptr)
	{
		return;
	}

	SkeletalMeshSocket->AttachActor(Weapon, SkeletalMeshComponent);

	BSTCharacterOwnerWeak->GetCharacterMovement()->bOrientRotationToMovement = false;
	BSTCharacterOwnerWeak->bUseControllerRotationYaw = true;

	if (OnEquippedWeaponChanged.IsBound())
	{
		OnEquippedWeaponChanged.Broadcast(Weapon);
	}
}

void UBSTCombatComponent::SetIsAiming(bool bIsAiming)
{
	if (bAiming == bIsAiming)
	{
		return;
	}

	bAiming = bIsAiming;

	if (OnAimChanged.IsBound())
	{
		OnAimChanged.Broadcast(bIsAiming);
	}
}

void UBSTCombatComponent::OnRep_EquippedWeaponWeak()
{
	if (OnEquippedWeaponChanged.IsBound())
	{
		OnEquippedWeaponChanged.Broadcast(EquippedWeaponWeak.Get());
	}

	if (EquippedWeaponWeak.IsValid())
	{
		BSTCharacterOwnerWeak->GetCharacterMovement()->bOrientRotationToMovement = false;
		BSTCharacterOwnerWeak->bUseControllerRotationYaw = true;
	}	
}

void UBSTCombatComponent::OnRep_bAiming()
{
	if (OnAimChanged.IsBound())
	{
		OnAimChanged.Broadcast(bAiming);
	}
}