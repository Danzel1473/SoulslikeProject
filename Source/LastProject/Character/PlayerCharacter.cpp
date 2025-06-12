// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"

#include "GameFramework/Character.h"
#include "CharacterControlData.h"
#include "ComboActionData.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "EnhancedInputComponent.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "TimerManager.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "LastProject/Items/WeaponBase.h"
#include "LastProject/Items/WeaponData.h"

class UAnimInstance;

APlayerCharacter::APlayerCharacter()
{
	// 매시 초기화
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMesh(TEXT("/Game/LastProject/QuangPhan/G2_Mercenaries/Meshes/Characters/Combines/SK_PP801_MercB.SK_PP801_MercB"));
	if (CharacterMesh.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMesh.Object);
	}
	
	// Animation BluePrint 연결
	static ConstructorHelpers::FClassFinder<UAnimInstance> CharacterAnim(TEXT("/Game/LastProject/Character/Animation/ABP_PlayerCharacter.ABP_PlayerCharacter_C"));
	if (CharacterAnim.Class)
	{
		GetMesh()->SetAnimClass(CharacterAnim.Class);
	}
	
	// 스프링암 설정
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->bUsePawnControlRotation = true;

	// 카메라 설정
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	// Input
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Game/LastProject/Input/IMC_PlayerCharacter.IMC_PlayerCharacter"));
	if (InputMappingContextRef.Object)
	{
		DefaultMappingContext = InputMappingContextRef.Object;
	}
	
	// Move Action
	static ConstructorHelpers::FObjectFinder<UInputAction> MoveActionRef(TEXT("/Game/LastProject/Input/InputAction/IA_Move.IA_Move"));
	if (MoveActionRef.Object)
	{
		MoveAction = MoveActionRef.Object;
	}

	// Look Action
	static ConstructorHelpers::FObjectFinder<UInputAction> LookActionRef(TEXT("/Game/LastProject/Input/InputAction/IA_Look.IA_Look"));
	if (LookActionRef.Object)
	{
		LookAction = LookActionRef.Object;
	}

	// 점프 액션 선언
	static ConstructorHelpers::FObjectFinder<UInputAction> JumpActionRef(TEXT("/Game/LastProject/Input/InputAction/IA_Jump.IA_Jump"));
	if (JumpActionRef.Object)
	{
		JumpAction = JumpActionRef.Object;
	}

	// 어택 액션 초기화
	static ConstructorHelpers::FObjectFinder<UInputAction> AttackActionRef(TEXT("/Game/LastProject/Input/InputAction/IA_Attack.IA_Attack"));
	if (AttackActionRef.Object)
	{
		AttackAction = AttackActionRef.Object;
	}

	// 방어 액션 초기화
	static ConstructorHelpers::FObjectFinder<UInputAction> GuardActionRef(TEXT(""));
	if (GuardActionRef.Object)
	{
		GuardAction = GuardActionRef.Object;
	}
	
	// 회피 액션 초기화
	static ConstructorHelpers::FObjectFinder<UInputAction> DodgeActionRef(TEXT("/Game/LastProject/Input/InputAction/IA_Dodge.IA_Dodge"));
	if (DodgeActionRef.Object)
	{
		DodgeAction = DodgeActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DodgeAnimMontageRef(TEXT("/Game/LastProject/Character/Animation/AM_Dodge.AM_Dodge"));
	if (DodgeAnimMontageRef.Object)
	{
		DodgeAnimMontage = DodgeAnimMontageRef.Object;
	}
	bIsDodging = false;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	

	// Add InputMapping Context to Enhanced Input System.
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (auto SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		SubSystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void APlayerCharacter::Attack()
{
	// 조건 체크
	//if (BattleState != BattleState::None || GetMovementComponent()->IsFalling()) return;
	
	// 콤보어택 처리
	ProcessComboCommand();
}

void APlayerCharacter::Guard()
{
	//if (BattleState != BattleState::None || GetMovementComponent()->IsFalling()) return;

	// Todo: 가드 로직
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	// 입력 값 읽기.
	FVector2D Movement = Value.Get<FVector2D>();

	// 컨트롤러의 회전 값.
	FRotator Rotation = GetControlRotation();
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

	// 방향 구하기.
	FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// 무브먼트 컴포넌트에 값 전달.
	AddMovementInput(ForwardVector, Movement.X);
	AddMovementInput(RightVector, Movement.Y);
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	// 입력 값 읽기.
	FVector2D LookVector = Value.Get<FVector2D>();

	// 컨트롤러에 회전 추가.
	AddControllerYawInput(-LookVector.X);
	AddControllerPitchInput(LookVector.Y);
}

void APlayerCharacter::Dodge(const FInputActionValue& Value)
{
	
	// 회피중/점프중에 회피 불가
	//if (bIsDodging || GetMovementComponent()->IsFalling() || bIsAttacking) return;
	
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// Dodge Montage 실행
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		BattleState = BattleState::Dodging;
		float SpeedRate = 1; // 추후 무게 값을 반영
		AnimInstance->Montage_Play(DodgeAnimMontage, SpeedRate);

		// Todo: 닷지중에 무적 상태 추가
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &APlayerCharacter::DodgeEnd);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, DodgeAnimMontage);
	}
}

void APlayerCharacter::DodgeEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	BattleState = BattleState::None;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void APlayerCharacter::ProcessComboCommand()
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


void APlayerCharacter::ComboActionBegin()
{
	// 어택중 플래그 설정
	bIsAttacking = true;
	
	CurrentCombo = 1;
	GetCharacterMovement()->SetMovementMode(MOVE_None);

	AttackInputDelay();
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		// 추후에 무게에 따른 공격속도 변경
		AnimInstance->Montage_Play(ComboAttackMontage);

		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &APlayerCharacter::ComboActionEnd);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboAttackMontage);

		ComboTimerHandle.Invalidate();
		SetComboCheckTimer();
	}
}

void APlayerCharacter::ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	// 유효성 검사
	ensure(CurrentCombo != 0);

	// 콤보 초기화
	CurrentCombo = 0;

	bIsAttacking = false;
	BattleState = BattleState::None;

	// 캐릭터 무브먼트 컴포넌트 모드 복구
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);

	// 공격이 끝나면 Notify 호출
	NotifyComboActionEnd();
}

void APlayerCharacter::NotifyComboActionEnd()
{
}

void APlayerCharacter::SetComboCheckTimer()
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
			&APlayerCharacter::ComboCheck,
			ComboEffectiveTime,
			false
			);
	}
}

void APlayerCharacter::ComboCheck()
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

void APlayerCharacter::AttackInputDelay()
{
	// 현재 재생 중인 콤보의 인덱스
	int32 ComboIndex = CurrentCombo - 1;

	// 콤보 인덱스 값 검증
	ensure(ComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	// 콤보 입력 딜레이 계산
	float ComboEffectiveTime =
		(ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate) / 3;
	
	// 어택 딜레이 플래그 설정
	bCanAttack = false;
	
	// 어택 딜레이 타이머 설정
	GetWorld()->GetTimerManager().SetTimer(
		AttackInputDelayTimerHandle,
		this,
		 &APlayerCharacter::ResetAttackTime,
		ComboEffectiveTime,
		false
	);
}

void APlayerCharacter::ResetAttackTime()
{
	bCanAttack = true;
}

void APlayerCharacter::SetEquippedWeapon(AWeaponBase* NewWeapon)
{
	if (!NewWeapon->GetMeshValid())
	{
		NewWeapon->AsyncLoadMesh();
	}
	
	// USkeletalMesh* LoadedMesh = NewWeapon->WeaponMesh.Get();
	// Weapon->SetSkeletalMesh(LoadedMesh);
	//
	FName SocketName = "RightHandSocket";

	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
}

// void APlayerCharacter::TakeItem(UWeaponData* InWeaponData)
// {
// }

void APlayerCharacter::EquipWeapon(class AWeaponBase)
{
	
}


void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	auto EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	// 바인딩
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
	//EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Attack);
	EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Dodge);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

}
