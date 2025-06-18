// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GruntAttack.h"

#include "AIController.h"
#include "LastProject/Character/CharacterBase.h"

class ACharacterBase;

UBTTask_GruntAttack::UBTTask_GruntAttack()
{
}

EBTNodeResult::Type UBTTask_GruntAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	ACharacterBase* CharacterBase = Cast<ACharacterBase>(OwnerComp.GetAIOwner()->GetPawn());

	if (!CharacterBase) return EBTNodeResult::Failed;

	CharacterBase->Attack();

	return EBTNodeResult::Succeeded;
}
