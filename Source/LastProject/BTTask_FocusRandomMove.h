// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FocusRandomMove.generated.h"

/**
 * 
 */
UCLASS()
class LASTPROJECT_API UBTTask_FocusRandomMove : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_FocusRandomMove();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

private:
	// 타이머 핸들러
	FTimerHandle TimerHandle;

	// 타이머 콜백
	void OnTimerFinished(UBehaviorTreeComponent* OwnerComp);

	// 타이머 지속 시간 (랜덤으로 설정)
	float Duration;

	// 랜덤 이동 방향 계산 함수
	FVector GetRandomMoveDirection(AActor* OwnerActor, FVector FocusLocation, float Radius = 300.f);
};