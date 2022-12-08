#include "Controllers/SecurityVideoCameraAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "UE5_Shooter/UE5_ShooterCharacter.h"
#include "UE5_Shooter/UE5_ShooterGameMode.h"

ASecurityVideoCameraAIController::ASecurityVideoCameraAIController()
{	
	SightSense = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightSense"));

	CCTVPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("CCTVPerceptionComponent"));
	SetPerceptionComponent(*CCTVPerceptionComponent);
	
	CCTVPerceptionComponent->SetDominantSense(*SightSense->GetSenseImplementation());
	CCTVPerceptionComponent->ConfigureSense(*SightSense);
}

void ASecurityVideoCameraAIController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (IsValid(CCTVPerceptionComponent))
	{
		CCTVPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ThisClass::OnPerceptionUpdate);
	}
	
	UWorld* World = GetWorld();
	if (IsValid(World))
	{
		AUE5_ShooterGameMode* ShooterGameMode = Cast<AUE5_ShooterGameMode>(World->GetAuthGameMode());
		if (IsValid(ShooterGameMode))
		{			
			ShooterGameMode->OnPlayerPerceptionChange.AddUObject(this, &ThisClass::OnPlayerPerspectiveChange);
		}
	}
}

void ASecurityVideoCameraAIController::OnPlayerPerspectiveChange(EPlayerPerceptionState NewState)
{
	if(IsValid(BlackboardComponent))
	{
		if (NewState == EPlayerPerceptionState::FPS)
		{
			BlackboardComponent->SetValueAsBool(FName("bUsingFPS"), true);
		}
		else
		{
			BlackboardComponent->SetValueAsBool(FName("bUsingFPS"), false);
		}
	}
}

void ASecurityVideoCameraAIController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(BehaviorTree);
	BlackboardComponent = GetBlackboardComponent();	 
}

void ASecurityVideoCameraAIController::OnPerceptionUpdate(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		AUE5_ShooterCharacter* ShooterCharacter = Cast<AUE5_ShooterCharacter>(Actor);
		if (IsValid(ShooterCharacter) && IsValid(BlackboardComponent))
		{
			BlackboardComponent->SetValueAsObject(FName("Player"), ShooterCharacter);
		}
	}
	else
	{
		if (IsValid(BlackboardComponent))
		{
			BlackboardComponent->ClearValue(FName("Player"));			
		}
	}
	
}