// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_Parryable.h"

#include "LastProject/Character/CharacterBase.h"

class ACharacterBase;

void UAnimNotifyState_Parryable::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                             float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	ACharacterBase* Owner = Cast<ACharacterBase>(MeshComp->GetOwner());

	if (!Owner) return;

	Owner->SetParryable(true);
}

void UAnimNotifyState_Parryable::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	ACharacterBase* Owner = Cast<ACharacterBase>(MeshComp->GetOwner());

	if (!Owner) return;

	Owner->SetParryable(true);
}
