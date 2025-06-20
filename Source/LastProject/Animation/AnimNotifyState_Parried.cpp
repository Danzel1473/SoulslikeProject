// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_Parried.h"

#include "LastProject/Character/CharacterBase.h"

class ACharacterBase;

void UAnimNotifyState_Parried::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                           float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	ACharacterBase* Owner = Cast<ACharacterBase>(MeshComp->GetOwner());
	
	if (!Owner) return;
	
	Owner->SetBattleState(BattleState::Parried);
	UE_LOG(LogTemp, Warning, TEXT("CalledFunction: %s / BattleState: %s"),
	TEXT(__FUNCTION__),
	*UEnum::GetValueAsString(Owner->GetBattleState())
);

}

void UAnimNotifyState_Parried::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	ACharacterBase* Owner = Cast<ACharacterBase>(MeshComp->GetOwner());
	
	if (!Owner) return;
	
	Owner->SetBattleState(BattleState::None);
	UE_LOG(LogTemp, Warning, TEXT("CalledFunction: %s / BattleState: %s"),
		TEXT(__FUNCTION__),
		*UEnum::GetValueAsString(Owner->GetBattleState())
	);

}
