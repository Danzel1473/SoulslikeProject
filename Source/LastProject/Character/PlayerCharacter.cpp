// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"

#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "EnhancedInputComponent.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "NonPlayerCharacter.h"
#include "TimerManager.h"
#include "Animation/AnimInstance.h"
#include "Blueprint/UserWidget.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"

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
	
	static ConstructorHelpers::FObjectFinder<UInputAction> ParryActionRef(TEXT("/Game/LastProject/Input/InputAction/IA_Parry.IA_Parry"));
	if (ParryActionRef.Object)
	{
		ParryAction = ParryActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DodgeAnimMontageRef(TEXT("/Game/LastProject/Character/Animation/AM_Dodge.AM_Dodge"));
	if (DodgeAnimMontageRef.Object)
	{
		DodgeAnimMontage = DodgeAnimMontageRef.Object;
	}

	LockOnComponent = CreateDefaultSubobject<ULockonComponent>(TEXT("LockOnComponent"));

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
	
	if (LockOnComponent)
	{
		LockOnComponent->OnLockOnModeChanged.AddDynamic(this, &APlayerCharacter::SetLockOnMovingMode);
	}
	
	if (WeaponCollision)
	{
		WeaponCollision->SetGenerateOverlapEvents(true);
		WeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnWeaponOverlapBegin);
	}
	
	if (PlayerHUDClass)
	{
		PlayerHUD = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDClass);
		if (PlayerHUD)
		{
			PlayerHUD->AddToViewport();
		}
	}
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (LockOnComponent->GetLockOnMode() && LockOnComponent->CurrentTarget)
	{
		if (!GetController()) return;
		FVector ToTarget = LockOnComponent->CurrentTarget->GetActorLocation() - GetActorLocation();
		//ToTarget.Z = 0.f; // 수평으로만 계산
		FRotator TargetRot = ToTarget.Rotation();

		GetController()->SetControlRotation(FMath::RInterpTo(GetController()->GetControlRotation(), TargetRot, DeltaSeconds, 16.f));
	}
}

void APlayerCharacter::Guard()
{
	// Todo: 가드 로직
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	// 입력 값 읽기
	FVector2D Movement = Value.Get<FVector2D>();

	if (BattleState != BattleState::None) return;

	if (LockOnComponent && LockOnComponent->GetLockOnMode())
	{
		// 락온 상태에서는 캐릭터 자체 방향 사용
		FVector Forward = GetActorForwardVector();
		FVector Right = GetActorRightVector();

		AddMovementInput(Forward, Movement.X);
		AddMovementInput(Right, Movement.Y);
	}
	else
	{
		// 컨트롤러의 회전 값
		FRotator Rotation = GetControlRotation();
		FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

		// 방향 구하기
		FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// 무브먼트 컴포넌트에 값 전달
		AddMovementInput(ForwardVector, Movement.X);
		AddMovementInput(RightVector, Movement.Y);
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	// 입력 값 읽기.
	FVector2D LookVector = Value.Get<FVector2D>();

	// 컨트롤러에 회전 추가.
	AddControllerYawInput(-LookVector.X);
	AddControllerPitchInput(LookVector.Y);
}

void APlayerCharacter::Dodge()
{
	if (BattleState != BattleState::None) return;
	
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		SetBattleState(BattleState::Dodging);
		
		UE_LOG(LogTemp, Warning, TEXT("CalledFunction: %s / BattleState: %s"), TEXT(__FUNCTION__), *UEnum::GetValueAsString(BattleState));

		
		
		float SpeedRate = 1.2; // 추후 무게 값을 반영
		
		// Dodge Montage 실행
		AnimInstance->Montage_Play(DodgeAnimMontage, SpeedRate);

		// 닷지중에 무적 상태
		IsInvincible = true;

		// 딜리게이트 추가
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &APlayerCharacter::DodgeEnd);
		
		AnimInstance->Montage_SetEndDelegate(EndDelegate, DodgeAnimMontage);
	}
}

void APlayerCharacter::Parry()
{
	if (BattleState == BattleState::None && !GetCharacterMovement()->IsFalling())
	{
		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			BattleState = BattleState::ParryMotion;
			float SpeedRate = 0.7; // 추후 무게 값을 반영
			
			// Dodge Montage 실행
			AnimInstance->Montage_Play(ParryAnimMontage, SpeedRate);

			// 딜리게이트 추가
			FOnMontageEnded EndDelegate;
			EndDelegate.BindUObject(this, &APlayerCharacter::ParryEnd);
			
			AnimInstance->Montage_SetEndDelegate(EndDelegate, ParryAnimMontage);
		}
	}
}

void APlayerCharacter::ParryEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	BattleState = BattleState::None;
}

void APlayerCharacter::DodgeEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	// 무적 상태 해제
	IsInvincible = false;

	// 상태 초기화
	BattleState = BattleState::None;

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void APlayerCharacter::SetLockOnMovingMode(bool IsLockOnMode)
{
	UE_LOG(LogInput, Log, TEXT("델리게이트 확인"));
	if (IsLockOnMode)
	{
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;

		if (GetController())
		{
			GetController()->SetIgnoreLookInput(true);
		}
	}
	else
	{
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;

		if (GetController())
		{
			GetController()->SetIgnoreLookInput(false);
		}
		
	}
}

void APlayerCharacter::OnWeaponOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this) return;

	ANonPlayerCharacter* HitEnemy = Cast<ANonPlayerCharacter>(OtherActor);
	if (!HitEnemy) return;

	// 테스트용 임시 데미지
	float DamageAmount = 10.f;
	UGameplayStatics::ApplyDamage(OtherActor, DamageAmount, GetController(), this, UDamageType::StaticClass());
}

void APlayerCharacter::ShowBossHPBar(ANonPlayerCharacter* BossCharacter)
{
	
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	auto EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	// 바인딩
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController && CameraShakeClass)
	{
		PlayerController->PlayerCameraManager->StartCameraShake(CameraShakeClass);
	}
	
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}
