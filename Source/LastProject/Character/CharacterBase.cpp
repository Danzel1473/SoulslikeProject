// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"

#include "CharacterControlData.h"
#include "ComboActionData.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 회전 제어
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 400.0f;

	GetCapsuleComponent()->SetCapsuleHalfHeight(88.0f);

	BattleState = BattleState::None;

	// 
	GetMesh()->SetRelativeLocationAndRotation(
		FVector(0.0f, 0.0f, -88.0f),
		FRotator(0.0f, -90.0f, 0.0f)
	);

	FName SocketName = TEXT("RightHandSocket");

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(GetMesh(), SocketName);

	WeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollision"));
	WeaponCollision->SetupAttachment(WeaponMesh);
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float ACharacterBase::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


void ACharacterBase::SetCharacterControlData(const class UCharacterControlData* InCharacterControlData)
{
	// Pawn.
	bUseControllerRotationYaw = InCharacterControlData->bUseControllerRotationYaw;

	// CharacterMovement.
	GetCharacterMovement()->bOrientRotationToMovement = InCharacterControlData->bOrientRotationToMovement;
	GetCharacterMovement()->bUseControllerDesiredRotation = InCharacterControlData->bUseControllerDesiredRotation;
	GetCharacterMovement()->RotationRate = InCharacterControlData->RotationRate;
}

void ACharacterBase::EnableWeaponCollision_Implementation()
{
	ICombatInterface::EnableWeaponCollision_Implementation();

	if (WeaponCollision)
	{
		WeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		UE_LOG(LogInput, Log, TEXT("WeaponCollision Status: %s"), WeaponCollision->GetCollisionEnabled() ? TEXT("Enabled") : TEXT("Disabled"));
	}
}

void ACharacterBase::DisableWeaponCollision_Implementation()
{
	ICombatInterface::DisableWeaponCollision_Implementation();
	
	if (WeaponCollision)
	{
		WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		UE_LOG(LogInput, Log, TEXT("WeaponCollision Status: %s"), WeaponCollision->GetCollisionEnabled() ? TEXT("Enabled") : TEXT("Disabled"));
	}
}

void ACharacterBase::HitBegin_Implementation()
{
	ICombatInterface::HitBegin_Implementation();

	BattleState = BattleState::Hit;
	GetCharacterMovement()->SetMovementMode(MOVE_None);
}

void ACharacterBase::HitEnd_Implementation()
{
	ICombatInterface::HitEnd_Implementation();
	
	BattleState = BattleState::None;
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

void ACharacterBase::Attack()
{
	ProcessComboCommand();
}

void ACharacterBase::ProcessComboCommand()
{
	BattleState = BattleState::Attacking;
	if (CurrentCombo == 0)
	{
		ComboActionBegin();
		return;
	}

	if (!ComboTimerHandle.IsValid())
	{
		HasNextComboCommand = false;
	}
	else if (bCanAttack)
	{
		HasNextComboCommand = true;
	}
}

void ACharacterBase::ComboActionBegin()
{
	CurrentCombo = 1;
	GetCharacterMovement()->SetMovementMode(MOVE_None);

	AttackInputDelay();
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		// 추후에 무게에 따른 공격속도 변경
		AnimInstance->Montage_Play(ComboAttackMontage);

		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &ACharacterBase::ComboActionEnd);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboAttackMontage);

		ComboTimerHandle.Invalidate();
		SetComboCheckTimer();
	}
}

void ACharacterBase::ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	// 유효성 검사
	ensure(CurrentCombo != 0);

	// 콤보 초기화
	CurrentCombo = 0;

	BattleState = BattleState::None;

	// 캐릭터 무브먼트 컴포넌트 모드 복구
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);

	// 공격이 끝나면 Notify 호출
	NotifyComboActionEnd();
}

void ACharacterBase::AttackInputDelay()
{
	// 현재 재생 중인 콤보의 인덱스
	int32 ComboIndex = CurrentCombo - 1;

	// 콤보 인덱스 값 검증
	ensure(ComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	// 콤보 입력 딜레이 계산
	float ComboEffectiveTime = (ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate) / 3;
	
	// 어택 딜레이 플래그 설정
	bCanAttack = false;
	
	// 어택 딜레이 타이머 설정
	GetWorld()->GetTimerManager().SetTimer(
		AttackInputDelayTimerHandle,
		this,
		 &ACharacterBase::ResetAttackTime,
		ComboEffectiveTime,
		false
	);
}

void ACharacterBase::NotifyComboActionEnd()
{
}

void ACharacterBase::SetComboCheckTimer()
{
	// 현재 재생 중인 콤보의 인덱스
	int32 ComboIndex = CurrentCombo - 1;

	// 콤보 인덱스 값 검증
	ensure(ComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	// 콤보 시간 계산 (추후에 공격속도 추가 계산)
	float ComboEffectiveTime = (ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate);

	// 타이머 설정
	if (ComboEffectiveTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(
			ComboTimerHandle,
			this,
			&ACharacterBase::ComboCheck,
			ComboEffectiveTime,
			false
			);
	}
}

void ACharacterBase::ComboCheck()
{
	UE_LOG(LogTemp, Display, TEXT("ComboCheck Start"));
	// 타이머 핸들 무효화
	ComboTimerHandle.Invalidate();

	// 이전 공격 입력 확인
	if (HasNextComboCommand)
	{
		UE_LOG(LogTemp, Display, TEXT("ComboCheck OK"));

		// 몽타주 점프 처리
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		if (AnimInstance)
		{
			// 다음 콤보 인덱스 설정
			CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionData->MaxComboCount);

			// 점프할 섹션의 이름 설정
			FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, CurrentCombo);

			// 섹션 점프
			AnimInstance->Montage_JumpToSection(NextSection, ComboAttackMontage);

			// 콤보 인풋 딜레이 설정
			AttackInputDelay();
			
			// 다음 콤보 공격을 위한 타이머 설정
			SetComboCheckTimer();

			// 콤보 입력 플래그 초기화
			HasNextComboCommand = false;
		}
	}
}

void ACharacterBase::ResetAttackTime()
{
	bCanAttack = true;
}

void ACharacterBase::ChangeMoveSpeed(const float Speed) const
{
	GetCharacterMovement()->MaxWalkSpeed = Speed;
	UE_LOG(LogInput, Log, TEXT("Walking speed: %f"), Speed);
}



