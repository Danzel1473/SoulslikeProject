// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GruntAttack.h"

#include "AIController.h"
#include "LastProject/Character/CharacterBase.h"
#include "LastProject/Interface//CharacterAIInterface.h"

class ACharacterBase;

UBTTask_GruntAttack::UBTTask_GruntAttack()
{
}

EBTNodeResult::Type UBTTask_GruntAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (!ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}
	
	ICharacterAIInterface* AIPawn 
		= Cast<ICharacterAIInterface>(ControllingPawn);

	if (!AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	ACharacterBase* OwnerCharacter = Cast<ACharacterBase>(AIPawn);
	if (OwnerCharacter &&  OwnerCharacter->GetBattleState() != BattleState::None)
	{
		return EBTNodeResult::Failed;
	}
	
	FAICharacterAttackFinished OnAttackFinished;

	OnAttackFinished.BindLambda(
		[&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);

	AIPawn->SetAIAttackDelegeate(OnAttackFinished);
	
	AIPawn->AttackByAI();

	return EBTNodeResult::InProgress;
}
