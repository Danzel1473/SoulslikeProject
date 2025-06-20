// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_Hit.h"

#include "LastProject/Character/CharacterBase.h"
#include "LastProject/Interface/CharacterAIInterface.h"

class ACharacterBase;

void UAnimNotifyState_Hit::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	ACharacterBase* Owner = Cast<ACharacterBase>(MeshComp->GetOwner());
	
	if (!Owner) return;
	
	Owner->SetBattleState(BattleState::Hit);
	UE_LOG(LogTemp, Warning, TEXT("CalledFunction: %s / BattleState: %s"),
	TEXT(__FUNCTION__),
	*UEnum::GetValueAsString(Owner->GetBattleState())
	);
	
	if (Owner->Implements<UCombatInterface>())
	{
		
		ICombatInterface::Execute_HitBegin(Owner);
	}
}

void UAnimNotifyState_Hit::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	ACharacterBase* Owner = Cast<ACharacterBase>(MeshComp->GetOwner());

	if (!Owner) return;
	Owner->SetBattleState(BattleState::None);
	
	if (Owner->Implements<UCombatInterface>())
	{

		ICombatInterface::Execute_HitEnd(Owner);
	}
}
