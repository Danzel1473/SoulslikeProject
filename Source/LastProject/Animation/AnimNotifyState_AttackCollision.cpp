// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_AttackCollision.h"

#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "LastProject/Character/CharacterBase.h"


void UAnimNotifyState_AttackCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	ACharacterBase* Owner = Cast<ACharacterBase>(MeshComp->GetOwner());

	if (!Owner) return;
	if (Owner->Implements<UCombatInterface>())
	{
		ICombatInterface::Execute_EnableWeaponCollision(Owner);
	}
}

void UAnimNotifyState_AttackCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	ACharacterBase* Owner = Cast<ACharacterBase>(MeshComp->GetOwner());

	if (!Owner) return;
	if (Owner->Implements<UCombatInterface>())
	{
		ICombatInterface::Execute_DisableWeaponCollision(Owner);
	}
}