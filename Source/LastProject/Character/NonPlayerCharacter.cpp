// Fill out your copyright notice in the Description page of Project Settings.


#include "NonPlayerCharacter.h"

#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

ANonPlayerCharacter::ANonPlayerCharacter()
{
	
}

void ANonPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

float ANonPlayerCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	if (HitMontage)
	{
		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			// DamageCauser를 바라보도록 함
			FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), DamageCauser->GetActorLocation());
			this->SetActorRotation(Rotator);
			
			// 피격 애니메이션 재생
			AnimInstance->Montage_Play(HitMontage, 1.5);
		}	
	}
	
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

// void ANonPlayerCharacter::Attack()
// {
// 	if (BattleState != BattleState::None) return;
//
// 	BattleState = BattleState::Attacking;
// 	Super::Attack();
//
// 	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
// 	if (AnimInstance && AttackMontage)
// 	{
// 		// 추후에 무게에 따른 공격속도 변경
// 		AnimInstance->Montage_Play(AttackMontage);
//
// 		FOnMontageEnded EndDelegate;
// 		EndDelegate.BindUObject(this, &ANonPlayerCharacter::AttackEnd);
// 		AnimInstance->Montage_SetEndDelegate(EndDelegate, AttackMontage);
// 	}
// }

// void ANonPlayerCharacter::AttackEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
// {
// 	BattleState = BattleState::None;
// }
