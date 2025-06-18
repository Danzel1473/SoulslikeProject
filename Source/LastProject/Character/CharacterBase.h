// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "LastProject/Interface/CombatInterface.h"
#include "CharacterBase.generated.h"

UENUM(BlueprintType)
enum class BattleState : uint8
{
	None = 0	UMETA(DisplayName = "None"),
	Attacking	UMETA(DisplayName = "Attacking"),
	Guarding	UMETA(DisplayName = "Guarding"),
	Dodging		UMETA(DisplayName = "Dodging"),
	Hit			UMETA(DisplayName = "Hit"),
};

UCLASS()
class LASTPROJECT_API ACharacterBase : public ACharacter, public ICombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

	void BeginPlay() override;

	void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;	
	virtual void SetCharacterControlData(const class UCharacterControlData* InCharacterControlData);

	virtual void EnableWeaponCollision_Implementation() override;
	virtual void DisableWeaponCollision_Implementation() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> HitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	TObjectPtr<UAnimMontage> ComboAttackMontage;
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	BattleState BattleState;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Equipment, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> WeaponMesh; 
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> WeaponCollision;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class AWeaponBase> Weapon;

	// Combat
public:
	UFUNCTION(BlueprintCallable, Category = Combat)
	virtual void Attack();
	
	virtual void ProcessComboCommand();
	virtual void ComboActionBegin();
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	void AttackInputDelay();
	void NotifyComboActionEnd();
	void SetComboCheckTimer();
	void ComboCheck();
	void ResetAttackTime();

protected:
	virtual void HitBegin_Implementation() override;
	virtual void HitEnd_Implementation() override;

	bool bCanAttack = true;

	// 현재 재생 중인 콤보 단계
	int32 CurrentCombo = 0;

	// 콤보에 사용할 데이터
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UComboActionData> ComboActionData;
	
	// 콤보 가능 여부를 판단하기 위한 타이머 핸들
	FTimerHandle ComboTimerHandle;
	FTimerHandle AttackInputDelayTimerHandle;

	bool HasNextComboCommand = false;

	// WalkSpeed
public:
	float WalkSpeed = 150.f;
	float RunSpeed = 300.f;

	UFUNCTION(BlueprintCallable)
	void ChangeMoveSpeed(float Speed) const;
};
