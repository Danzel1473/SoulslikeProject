// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "LastProject/Player/LockonComponent.h"
#include "PlayerCharacter.generated.h"

// DECLARE_DELEGATE_OneParam(FOnTakeItemDelegate, class UWeaponData* /*InItemData*/);
//
// USTRUCT(BlueprintType)
// struct FTakeItemDelegateWrapper
// {
// 	GENERATED_BODY()
//
// 	FTakeItemDelegateWrapper() {}
// 	FTakeItemDelegateWrapper(const FOnTakeItemDelegate& InItemDelegate) : ItemDelegate(InItemDelegate) {}
//
// 	FOnTakeItemDelegate ItemDelegate;
// };

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

	virtual void Tick(float DeltaSeconds) override;
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

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
	TObjectPtr<UInputAction> ParryAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	TObjectPtr<UAnimMontage> DodgeAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	TObjectPtr<UAnimMontage> ParryAnimMontage;

	void Guard();

	UFUNCTION(BlueprintCallable)
	void Move(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable)
	void Look(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable)
	void Dodge();
	//void PlayerJump(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable)
	void Parry();
	
	void DodgeEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded);
	void ParryEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded);

	UFUNCTION()
	void SetLockOnMovingMode(bool IsLockOnMode);

public:
	void SetComboAttackMontage(UAnimMontage* NewMontage) { ComboAttackMontage = NewMontage; }

	// Combat
protected:
	UFUNCTION()
	void OnWeaponOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	// 콤보 타이머 이전에 입력이 들어왔는지를 확인하는 불리언 변수
	bool HasNextComboCommand = false;
	
	// LockOn
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = LockOn, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ULockonComponent> LockOnComponent;

	// CameraShake
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	TSubclassOf<UCameraShakeBase> CameraShakeClass;
	
};