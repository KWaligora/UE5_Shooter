#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "SecurityVideoCameraAIController.generated.h"

/**
 * 
 */
UCLASS()
class UE5_SHOOTER_API ASecurityVideoCameraAIController : public AAIController
{
	GENERATED_BODY()

public:
	ASecurityVideoCameraAIController();
	virtual void PostInitializeComponents() override;

		
protected:
	UPROPERTY(EditDefaultsOnly, Category="AI")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	UAIPerceptionComponent* CCTVPerceptionComponent;

	UPROPERTY()
	class UAISenseConfig_Sight* SightSense;

	UPROPERTY()
	UBlackboardComponent* BlackboardComponent;

	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnPerceptionUpdate(AActor* Actor, FAIStimulus Stimulus);
};
