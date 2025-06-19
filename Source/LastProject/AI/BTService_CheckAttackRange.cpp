// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_CheckAttackRange::UBTService_CheckAttackRange()
{
	Interval = 0.1f;
}

void UBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* AIPawn = AIController->GetPawn();

	AActor* Target = Cast<AActor>(BB->GetValueAsObject("TargetObject"));
	if (!Target || ! AIPawn) return;

	float Distance = AIPawn->GetDistanceTo(Target);
	bool bInRange = Distance < CanAttackRange;
	
	BB->SetValueAsBool("bIsInAttackRange", bInRange);
}
