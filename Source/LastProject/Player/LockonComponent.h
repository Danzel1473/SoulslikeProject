// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LockonComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LASTPROJECT_API ULockonComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULockonComponent();

	// virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	// 현재 락온 대상
	UPROPERTY(BlueprintReadOnly)
	AActor* CurrentTarget;

	// 탐지 거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxDistance = 1200.f;

	// 시야각
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FOV = 90.f;

	// 락온 대상자들
	UPROPERTY(BlueprintReadOnly)
	TArray<AActor*> Candidates;
	// 대상 탐지
	UFUNCTION(BlueprintCallable)
	void DetectLockOnTarget();

	// 타겟대상들 정렬(왼->오)
	UFUNCTION(BlueprintCallable)
	void SortCandidatesLeftToRight();
	
	// 락온 시작/종료
	UFUNCTION(BlueprintCallable)
	void ToggleLockOn();

	// 락온 대상 전환
	UFUNCTION(BlueprintCallable)
	void SwitchTarget(bool bRight);
	
protected:
	// 락온 상태
	bool bIsLockOnMode = false;
};
