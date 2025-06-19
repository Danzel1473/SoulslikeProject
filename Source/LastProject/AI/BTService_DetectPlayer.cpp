// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_DetectPlayer.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UBTService_DetectPlayer::UBTService_DetectPlayer()
{
	NodeName = "Detect Player";
	Interval = 0.3f; // 주기적으로 체크
}

void UBTService_DetectPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	AAIController* AIController = OwnerComp.GetAIOwner();

	if (!BB || !AIController) return;

	AActor* SelfActor = AIController->GetPawn();
	AActor* TargetActor = Cast<AActor>(BB->GetValueAsObject("TargetActor"));

	if (!SelfActor || !TargetActor) return;

	// 마지막으로 본 위치만 저장
	BB->SetValueAsVector("LastSeenTargetLocation", TargetActor->GetActorLocation());
}
