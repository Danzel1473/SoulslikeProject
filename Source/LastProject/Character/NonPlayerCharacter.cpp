// Fill out your copyright notice in the Description page of Project Settings.


#include "NonPlayerCharacter.h"

#include "DamageManager.h"
#include "PlayerCharacter.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ANonPlayerCharacter::ANonPlayerCharacter()
{
}

void ANonPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	
	if (WeaponCollision)
	{
		WeaponCollision->SetGenerateOverlapEvents(true);
		WeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &ANonPlayerCharacter::OnWeaponOverlapBegin);
	}
}

float ANonPlayerCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	if (GetEnemyType() == EnemyType::Boss)
	{
		UE_LOG(LogTemp, Log, TEXT("BossDamaged"));
		return ACharacter::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	}
		
	// DamageCauser를 바라보도록 함
	const FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), DamageCauser->GetActorLocation());
	this->SetActorRotation(Rotator);
	
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ANonPlayerCharacter::OnWeaponOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this) return;

	UE_LOG(LogTemp, Log, TEXT("Enemy Weapon Overlapped"))
	APlayerCharacter* HitPlayer = Cast<APlayerCharacter>(OtherActor);
	if (!HitPlayer || HitPlayer->GetIsInvincible()) return;

	if (HitPlayer->GetBattleState() == BattleState::Parrying && IsParryable)
	{
		SetParryable(false);
		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			AnimInstance->Montage_Stop(0.2f, AnimInstance->GetCurrentActiveMontage());

			if (ParriedMontage)
			{
				AnimInstance->Montage_Play(ParriedMontage, 1.f);
				
				// FOnMontageEnded EndDelegate;
				// EndDelegate.BindUObject(this, &ANonPlayerCharacter::ParriedEnd);
				// AnimInstance->Montage_SetEndDelegate(EndDelegate, ParriedMontage);
			}
		}
	}
	else
	{
		// 테스트용 임시 데미지
		//int32 DamageAmount = DamageManager->StatData->PowerStat;
		int32 DamageAmount = 10;
		UGameplayStatics::ApplyDamage(OtherActor, static_cast<float>(DamageAmount), GetController(), this, UDamageType::StaticClass());
	}
}

void ANonPlayerCharacter::ParriedEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	UE_LOG(LogInput, Log, TEXT("ParriedEnd"));
	SetBattleState(BattleState::None);
	//SetParryable(false);
}

void ANonPlayerCharacter::SetAIAttackDelegeate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void ANonPlayerCharacter::AttackByAI()
{
	if (BattleState == BattleState::None || BattleState == BattleState::Attacking)
	ProcessComboCommand();
}

void ANonPlayerCharacter::FoucsRandomMove()
{
}

void ANonPlayerCharacter::NotifyComboActionEnd()
{
	Super::NotifyComboActionEnd();

	OnAttackFinished.ExecuteIfBound();
}
