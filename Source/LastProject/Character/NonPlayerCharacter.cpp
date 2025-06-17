// Fill out your copyright notice in the Description page of Project Settings.


#include "NonPlayerCharacter.h"

#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

ANonPlayerCharacter::ANonPlayerCharacter()
{
	
}

float ANonPlayerCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	if (HitMontage)
	{
		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			// DamageCauser를 바라보도록 함
			FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), DamageCauser->GetActorLocation());
			this->SetActorRotation(Rotator);
			
			// 피격 애니메이션 재생
			AnimInstance->Montage_Play(HitMontage, 1);
		}	
	}
	
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}
