// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_Parry.h"

#include "LastProject/Character/CharacterBase.h"

void UAnimNotifyState_Parry::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                         float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	ACharacterBase* Owner = Cast<ACharacterBase>(MeshComp->GetOwner());

	if (!Owner) return;

	Owner->SetBattleState(BattleState::Parrying);
}

void UAnimNotifyState_Parry::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	ACharacterBase* Owner = Cast<ACharacterBase>(MeshComp->GetOwner());

	if (!Owner) return;

	Owner->SetBattleState(BattleState::ParryMotion);
}
