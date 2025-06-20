// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_TurnToTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "LastProject/Character/CharacterBase.h"
#include "LastProject/Character/NonPlayerCharacter.h"

class ACharacterBase;

UBTService_TurnToTarget::UBTService_TurnToTarget()
{
	Interval = 0.1f;
}

void UBTService_TurnToTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// AiController가 제어하는 폰
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControllingPawn)
	{
		return;
	}

	// 플레이어
	APawn* TargetPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetObject")
	);

	if (!TargetPawn)
	{
		return;
	}
	
	// 회전할 속도 값
	ANonPlayerCharacter* Character = Cast<ANonPlayerCharacter>(OwnerComp.GetAIOwner()->GetPawn());

	float TurnSpeed = Character->GetAITurnSpeed();

	// 바라볼 방향 구하기
	FVector LookVector
		= TargetPawn->GetActorLocation() - ControllingPawn->GetActorLocation();

	// 높이 성분은 제거
	LookVector.Z = 0.0f;

	// 바라볼 방향을 이용해 FRotator 구하기
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();

	// 부드럽게 회전 설정
	// 현재 프레임에 설정할 회전 값
	FRotator CurrentRot = FMath::RInterpTo(
		ControllingPawn->GetActorRotation(),
		TargetRot,
		GetWorld()->DeltaTimeSeconds,
		TurnSpeed
	);

	// 회전 값 설정
	ControllingPawn->SetActorRotation(CurrentRot);
}
