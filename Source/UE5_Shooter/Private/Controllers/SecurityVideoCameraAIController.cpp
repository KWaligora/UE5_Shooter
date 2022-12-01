#include "Controllers/SecurityVideoCameraAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "UE5_Shooter/UE5_ShooterCharacter.h"

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
}

void ASecurityVideoCameraAIController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(BehaviorTree);
	BlackboardComponent = GetBlackboardComponent();
}

void ASecurityVideoCameraAIController::OnPerceptionUpdate(AActor* Actor, FAIStimulus Stimulus)
{
	AUE5_ShooterCharacter* ShooterCharacter = Cast<AUE5_ShooterCharacter>(Actor);
	if (IsValid(ShooterCharacter) && IsValid(BlackboardComponent))
	{
		BlackboardComponent->SetValueAsObject("Player", ShooterCharacter);			
	}	
}