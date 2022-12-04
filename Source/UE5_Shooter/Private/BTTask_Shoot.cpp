// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_Shoot.h"

#include "SecurityVideoCamera.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/ShooterComponent.h"

UBTTask_Shoot::UBTTask_Shoot()
{
	NodeName = FString("Shoot");
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AController* MyController = Cast<AController>(OwnerComp.GetOwner());
	if (IsValid(MyController))
	{
		ASecurityVideoCamera* SecurityVideoCamera = Cast<ASecurityVideoCamera>(MyController->GetPawn());
		if (IsValid(SecurityVideoCamera))
		{
			UShooterComponent* ShooterComponent = SecurityVideoCamera->GetShooterComponent();
			if (IsValid (ShooterComponent))
			{
				UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
				if (IsValid(BlackboardComponent))
				{
					AActor* Target = Cast<AActor>(BlackboardComponent->GetValueAsObject(FName("Player")));
					ShooterComponent->AIShoot(Target);
					return EBTNodeResult::Succeeded;
				}			
			}		
		}
	}
	return EBTNodeResult::Failed;
}
