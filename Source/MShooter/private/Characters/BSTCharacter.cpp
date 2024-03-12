#include "Characters/BSTCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/BSTCombatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"

ABSTCharacter::ABSTCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetMesh());
	SpringArmComponent->TargetArmLength = 600.0f;
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	CombatComponent = CreateDefaultSubobject<UBSTCombatComponent>(TEXT("CombatComponent"));
	CombatComponent->SetIsReplicated(true);
}

void ABSTCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (PlayerInputComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ABSTCharacter::SetupPlayerInputComponent - invalid PlayerInputComponent"));
		return;
	}

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Equip"), IE_Pressed, this, &ABSTCharacter::ServerEquipBtnPressed);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ABSTCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ABSTCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ABSTCharacter::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ABSTCharacter::LookUp);
}

void ABSTCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ABSTCharacter, OverlappingWeapon, COND_OwnerOnly);
}

void ABSTCharacter::SetOverlappingWeapon_Sv(ABSTWeapon* Weapon)
{
	if (!HasAuthority())
	{
		return;
	}

	if (IsLocallyControlled())
	{
		if (IsValid(OverlappingWeapon))
		{
			OverlappingWeapon->ShowPickupWidget(false);
		}
		
		if (IsValid(Weapon))
		{
			Weapon->ShowPickupWidget(true);
		}
	}

	OverlappingWeapon = Weapon;
}

void ABSTCharacter::MoveForward(float Value)
{
	if (Controller == nullptr || Value == 0.0f)
	{
		return;
	}

	const FRotator YawRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
	const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
	AddMovementInput(Direction, Value);
}

void ABSTCharacter::MoveRight(float Value)
{
	if (Controller == nullptr || Value == 0.0f)
	{
		return;
	}

	const FRotator YawRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
	const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));
	AddMovementInput(Direction, Value);
}

void ABSTCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void ABSTCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void ABSTCharacter::ServerEquipBtnPressed_Implementation()
{
	if (CombatComponent != nullptr)
	{
		CombatComponent->EquipWeapon(OverlappingWeapon);
	}
}

void ABSTCharacter::OnRep_OverlappingWeapon(ABSTWeapon* LastWeapon)
{
	if (IsValid(LastWeapon))
	{
		LastWeapon->ShowPickupWidget(false);
	}
	
	if (!IsValid(OverlappingWeapon))
	{
		return;
	}

	OverlappingWeapon->ShowPickupWidget(true);
}