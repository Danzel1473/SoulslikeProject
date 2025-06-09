// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"

#include "GameFramework/Character.h"
#include "CharacterControlData.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "EnhancedInputComponent.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
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
	static ConstructorHelpers::FObjectFinder<UInputAction> AttackActionRef(TEXT(""));
	if (AttackActionRef.Object)
	{
		AttackAction = AttackActionRef.Object;
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

	isDodging = false;
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
	// Todo: 어택 구현
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
	if (isDodging || GetMovementComponent()->IsFalling()) return;
	
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	// Todo: 닷지중에 무적 상태 추가

	// Dodge Montage 실행
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		isDodging = true;
		
		float SpeedRate = 1; // 추후 무게 값을 반영
		AnimInstance->Montage_Play(DodgeAnimMontage, SpeedRate);

		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &APlayerCharacter::DodgeEnd);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, DodgeAnimMontage);
	}
}

void APlayerCharacter::DodgeEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	isDodging = false;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void APlayerCharacter::SetEquippedWeapon(UWeaponData* NewWeapon)
{
	if (!NewWeapon->WeaponMesh.IsValid())
	{
		NewWeapon->WeaponMesh.LoadSynchronous();
	}
	
	USkeletalMesh* LoadedMesh = NewWeapon->WeaponMesh.Get();
	Weapon->SetSkeletalMesh(LoadedMesh);

	FName SocketName = "RightHandSocket";

	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
}


void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	auto EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	// 바인딩
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Attack);
	EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Dodge);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

}
