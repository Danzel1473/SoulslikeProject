// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GruntAttack.generated.h"

/**
 * 
 */
UCLASS()
class LASTPROJECT_API UBTTask_GruntAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_GruntAttack();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
