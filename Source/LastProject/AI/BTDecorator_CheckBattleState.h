// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CheckBattleState.generated.h"

/**
 * 
 */
UCLASS()
class LASTPROJECT_API UBTDecorator_CheckBattleState : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_CheckBattleState();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
