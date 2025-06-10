// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "LastProject/Items/LPWeapon.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class LASTPROJECT_API APlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	virtual void BeginPlay() override;
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// Camera
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> Camera;

	// Input
protected:
	UPROPERTY(EditAnywhere, Category="Input")
	UInputMappingContext* IMC_Defalut;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IA_Move;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IA_Jump;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> GuardAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> DodgeAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	TObjectPtr<UAnimMontage> DodgeAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	TObjectPtr<UAnimMontage> ComboAttackMontage;
	
	bool bIsDodging;
	bool bIsAttacking;
	bool bCanAttack = true;

	UFUNCTION(BlueprintCallable, Category = Combat, meta = (AllowPrivateAccess = "true"))
	void Attack();
	
	void Guard();

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Dodge(const FInputActionValue& Value);
	void DodgeEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded);

public:
	void SetComboAttackMontage(UAnimMontage* NewMontage) { ComboAttackMontage = NewMontage; }

	// Combat
protected:
	// 현재 재생 중인 콤보 단계
	int32 CurrentCombo = 0;

	// 콤보에 사용할 데이터
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UComboActionData> ComboActionData;
	
	// 콤보 가능 여부를 판단하기 위한 타이머 핸들
	FTimerHandle ComboTimerHandle;
	FTimerHandle AttackInputDelayTimerHandle;

	// 콤보 액션 처리 함수
	void ProcessComboCommand();
	
	void AttackInputDelay();

	// 콤보 액션 시작 함수
	void ComboActionBegin();

	// 콤보 액션 종료 함수
	void ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded);
	
	// NPC가 공격이 끝나는 지점을 알 수 있도록 함수 추가
	virtual void NotifyComboActionEnd();

	// 콤보 타이머 설정 함수
	void SetComboCheckTimer();

	// 타이머 시간 사이에 입력이 들어왔는지 여부를 확인하는 함수
	void ComboCheck();

	void ResetAttackTime();
	
	// 콤보 타이머 이전에 입력이 들어왔는지를 확인하는 불리언 변수
	bool HasNextComboCommand = false;
	
	// Equipment
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UFUNCTION()
	void SetEquippedWeapon(UWeaponData* NewWeapon);
};