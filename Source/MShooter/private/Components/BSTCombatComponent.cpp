#include "Components/BSTCombatComponent.h"

#include "Weapons/BSTWeapon.h"
#include "Characters/BSTCharacter.h"
#include "Engine/SkeletalMeshSocket.h"

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

void UBSTCombatComponent::EquipWeapon(ABSTWeapon* Weapon)
{
	if (!BSTCharacterOwnerWeak.IsValid() || !IsValid(Weapon))
	{
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
}