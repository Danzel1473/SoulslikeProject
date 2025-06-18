// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class LASTPROJECT_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY()
	FTimerHandle TargetLocationUpdateTimer;
	FTimerHandle LoseSightTimerHandle;
	
	UPROPERTY()
	TObjectPtr<class UAISenseConfig_Sight> SightConfig;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<class UBehaviorTree> BehaviorTree;
	
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};
