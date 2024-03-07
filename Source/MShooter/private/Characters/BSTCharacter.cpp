#include "Characters/BSTCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

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
	
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ABSTCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ABSTCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ABSTCharacter::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ABSTCharacter::LookUp);
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

