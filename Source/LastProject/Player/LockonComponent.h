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

	/** 현재 락온 대상 */
	UPROPERTY(BlueprintReadOnly)
	AActor* CurrentTarget;

	/** 적 탐지 거리 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxDistance = 1000.f;

	/** 시야각 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FOV = 90.f;

	/** Enemy 채널로 필터링 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ECollisionChannel> EnemyObjectChannel;

	/** 현재 가능한 대상들 */
	UPROPERTY(BlueprintReadOnly)
	TArray<AActor*> Candidates;

	/** 대상 탐지 */
	UFUNCTION(BlueprintCallable)
	void DetectLockOnTarget();

	/** 락온 시작 또는 해제 */
	UFUNCTION(BlueprintCallable)
	void ToggleLockOn();

	/** 대상 전환 */
	UFUNCTION(BlueprintCallable)
	void SwitchTarget(bool bRight);
	
protected:
};
