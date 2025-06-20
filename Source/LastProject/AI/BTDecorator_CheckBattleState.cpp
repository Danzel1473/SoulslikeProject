// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CheckBattleState.h"

#include "AIController.h"
#include "LastProject/Character/CharacterBase.h"

UBTDecorator_CheckBattleState::UBTDecorator_CheckBattleState()
{
	NodeName = TEXT("Check BattleState");
}

bool UBTDecorator_CheckBattleState::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) const
{
	ACharacterBase* Character = Cast<ACharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Character)
	{
		return false;
	}

	return Character->GetBattleState() == BattleState::None;
}
