// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_Hit.h"

#include "LastProject/Character/CharacterBase.h"

class ACharacterBase;

void UAnimNotifyState_Hit::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	ACharacterBase* Owner = Cast<ACharacterBase>(MeshComp->GetOwner());

	UAnimInstance* AnimInstance = Owner->GetMesh()->GetAnimInstance();

	if (!Owner) return;
	if (Owner->Implements<UCombatInterface>())
	{
		AnimInstance->StopAllMontages(0.1f);
		if (Owner->HitMontage)
		{
			AnimInstance->Montage_Play(Owner->HitMontage);
		}
		ICombatInterface::Execute_HitBegin(Owner);
	}
}

void UAnimNotifyState_Hit::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	ACharacterBase* Owner = Cast<ACharacterBase>(MeshComp->GetOwner());

	if (!Owner) return;
	if (Owner->Implements<UCombatInterface>())
	{
		ICombatInterface::Execute_HitEnd(Owner);
	}
}
