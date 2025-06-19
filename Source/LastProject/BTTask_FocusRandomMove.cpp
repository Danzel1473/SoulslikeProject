// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FocusRandomMove.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

class UNavigationSystemV1;

UBTTask_FocusRandomMove::UBTTask_FocusRandomMove()
{
	NodeName = TEXT("Focus Random Move");
	bNotifyTick = false; // Tick 사용 안함
}

EBTNodeResult::Type UBTTask_FocusRandomMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
		return EBTNodeResult::Failed;

	ACharacter* OwnerCharacter = Cast<ACharacter>(AIController->GetPawn());
	if (!OwnerCharacter)
		return EBTNodeResult::Failed;

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
		return EBTNodeResult::Failed;

	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TEXT("TargetObject")));
	if (!TargetActor)
		return EBTNodeResult::Failed;

	// Focus 위치 (보통 플레이어 위치)
	FVector FocusLocation = TargetActor->GetActorLocation();

	// 랜덤 방향 구함
	FVector MoveDirection = GetRandomMoveDirection(OwnerCharacter, FocusLocation);

	// 목적지 계산 (Focus 위치 기준, 일정 거리 랜덤)
	FVector Destination = FocusLocation + MoveDirection * 300.f;

	// NavMesh 에서 유효한 위치 찾기
	FNavLocation NavLocation;
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(OwnerCharacter->GetWorld());
	if (NavSys && NavSys->GetRandomPointInNavigableRadius(Destination, 150.f, NavLocation))
	{
		Destination = NavLocation.Location;
	}

	// MoveTo 호출 (비동기)
	AIController->MoveToLocation(Destination);

	// 2~3초 랜덤 대기 시간 설정
	Duration = FMath::RandRange(2.0f, 3.0f);

	// 타이머 시작 (완료시 OnTimerFinished 호출)
	OwnerCharacter->GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateUObject(this, &UBTTask_FocusRandomMove::OnTimerFinished, &OwnerComp), Duration, false);

	return EBTNodeResult::InProgress;
}

void UBTTask_FocusRandomMove::OnTimerFinished(UBehaviorTreeComponent* OwnerComp)
{
	if (!OwnerComp) return;

	// 타이머 종료 후 Task 성공 알림
	FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);

	// AI 컨트롤러 멈춤
	AAIController* AIController = OwnerComp->GetAIOwner();
	if (AIController)
	{
		AIController->StopMovement();
	}
}

FVector UBTTask_FocusRandomMove::GetRandomMoveDirection(AActor* OwnerActor, FVector FocusLocation, float Radius)
{
	if (!OwnerActor)
		return FVector::ZeroVector;

	// Focus 위치에서 주위 방향 랜덤 (2D)
	FVector ToOwner = OwnerActor->GetActorLocation() - FocusLocation;
	ToOwner.Z = 0.f;
	ToOwner.Normalize();

	// 0~360도 랜덤 회전
	float RandomAngle = FMath::FRandRange(0.f, 360.f);
	FVector RandomDir = FVector(FMath::Cos(FMath::DegreesToRadians(RandomAngle)), FMath::Sin(FMath::DegreesToRadians(RandomAngle)), 0.f);

	// 방향 보정 (Focus에서 Owner로 향하는 방향 기반 랜덤)
	FVector Result = (ToOwner + RandomDir).GetSafeNormal();

	return Result;
}

void UBTTask_FocusRandomMove::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	// 타이머 클리어 (혹시 남아있으면)
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController && AIController->GetPawn())
	{
		AIController->GetPawn()->GetWorldTimerManager().ClearTimer(TimerHandle);
	}

	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}