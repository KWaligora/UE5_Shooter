// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE5_ShooterCharacter.h"

#include "UE5_ShooterGameMode.h"
#include "Actors/Projectiles/Projectile.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/ShooterComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

//////////////////////////////////////////////////////////////////////////
// AUE5_ShooterCharacter

AUE5_ShooterCharacter::AUE5_ShooterCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	// set our turn rate for input
	TurnRateGamepad = 50.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	TPSCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("TPSCameraBoom"));
	TPSCameraBoom->SetupAttachment(RootComponent);
	TPSCameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	TPSCameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	TPSCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowTPSCamera"));
	TPSCamera->SetupAttachment(TPSCameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	TPSCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	FPSCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCamera"));
	FPSCamera->SetupAttachment(GetMesh());
	FPSCamera->bUsePawnControlRotation = true;
	
	ShooterComponent = CreateDefaultSubobject<UShooterComponent>(TEXT("ShooterComponent"));

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AUE5_ShooterCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AUE5_ShooterCharacter::PrepareToShoot);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AUE5_ShooterCharacter::Shoot);

	PlayerInputComponent->BindAction("TogglePerspective", IE_Pressed, this, &AUE5_ShooterCharacter::TogglePerspective);

	PlayerInputComponent->BindAction("ZoomIn", IE_Pressed, this, &AUE5_ShooterCharacter::ZoomIn);
	PlayerInputComponent->BindAction("ZoomOut", IE_Pressed, this, &AUE5_ShooterCharacter::ZoomOut);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AUE5_ShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AUE5_ShooterCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &AUE5_ShooterCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &AUE5_ShooterCharacter::LookUpAtRate);
}

void AUE5_ShooterCharacter::ZoomIn()
{
	if(IsValid(FPSCamera) && FPSCamera->IsActive())
	{
		FPSCamera->SetFieldOfView(45.0f);
	}
}

void AUE5_ShooterCharacter::ZoomOut()
{
	if(IsValid(FPSCamera) && FPSCamera->IsActive())
	{
		FPSCamera->SetFieldOfView(90.0f);
	}
}


void AUE5_ShooterCharacter::SetupProjectilePrediction()
{
	UWorld* World = GetWorld();
	if (IsValid(World))
	{
		if (IsValid(ShooterComponent))
		{
			FPredictProjectilePathParams PredictProjectilePathParams;
			ShooterComponent->GetPredictProjectilePathParams(PredictProjectilePathParams);

			TArray<FVector> Points;

			FPredictProjectilePathResult PredictProjectilePathResult;
			for (int i=0; i<5; i++)
			{
				UGameplayStatics::PredictProjectilePath(World, PredictProjectilePathParams, PredictProjectilePathResult);				
				
				if(PredictProjectilePathResult.HitResult.IsValidBlockingHit())
				{					
					Points.Add(PredictProjectilePathResult.HitResult.ImpactPoint);					
					
					FVector NewVelocity = PredictProjectilePathResult.HitResult.ImpactNormal.GetSafeNormal() * PredictProjectilePathResult.LastTraceDestination.Velocity.Length();
					
					PredictProjectilePathParams.StartLocation = PredictProjectilePathResult.HitResult.ImpactPoint;
					PredictProjectilePathParams.LaunchVelocity = NewVelocity;													
				}				
			}
			if (Points.Num() > 2)
			{
				for (int i=0; i<Points.Num() - 1; i++)
				{
					DrawDebugLine(World, Points[i], Points[i+1], FColor::Red, false, -1, 0,2.0f);
				}				
			}	
		}
	}	
}

void AUE5_ShooterCharacter::PrepareToShoot()
{
	SetActorTickEnabled(true);
}

void AUE5_ShooterCharacter::Shoot()
{
	SetActorTickEnabled(false);
	
	if(IsValid(ShooterComponent))
	{
		ShooterComponent->PlayerShoot();
	}
}

void AUE5_ShooterCharacter::ActivateFPSCamera()
{
	FPSCamera->SetActive(true);
	TPSCamera->SetActive(false);

	bUseControllerRotationYaw = true;	

	GetCharacterMovement()->bOrientRotationToMovement = false;
}

void AUE5_ShooterCharacter::ActivateTPSCamera()
{
	FPSCamera->SetActive(false);
	TPSCamera->SetActive(true);
	
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AUE5_ShooterCharacter::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                  FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->IsA(AProjectile::StaticClass()))
	{
		Controller->UnPossess();
		GetMesh()->SetSimulatePhysics(true);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SetLifeSpan(5.0f);
	}
}

void AUE5_ShooterCharacter::TogglePerspective()
{
	if (IsValid(ShooterGameMode))
	{
		ShooterGameMode->TogglePerspective();

		if (TPSCamera->IsActive())
		{
			ActivateFPSCamera();
		}
		else
		{
			ActivateTPSCamera();	
		}
	}
}

void AUE5_ShooterCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AUE5_ShooterCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

AUE5_ShooterGameMode* AUE5_ShooterCharacter::GetGameMode()
{
	UWorld* World = GetWorld();
	if (IsValid(World))
	{
		return Cast<AUE5_ShooterGameMode>(World->GetAuthGameMode());
	}
	return nullptr;
}

void AUE5_ShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	ShooterGameMode = GetGameMode();
	
	UCapsuleComponent* CP = GetCapsuleComponent();
	if (IsValid(CP))
	{
		CP->OnComponentHit.AddDynamic(this, &ThisClass::OnHit);
	}
}

void AUE5_ShooterCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	SetupProjectilePrediction();
}

void AUE5_ShooterCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AUE5_ShooterCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
