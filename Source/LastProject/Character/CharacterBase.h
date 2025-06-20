// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterStatData.h"
#include "DamageManager.h"
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
	Parrying	UMETA(DisplayName = "Parrying"),
	ParryMotion	UMETA(DisplayName = "ParryMotion"),
	Parried		UMETA(DisplayName = "Parried"),

};

UCLASS()
class LASTPROJECT_API ACharacterBase : public ACharacter, public ICombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;	
	virtual void SetCharacterControlData(const class UCharacterControlData* InCharacterControlData);

	virtual void EnableWeaponCollision_Implementation() override;
	virtual void DisableWeaponCollision_Implementation() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> HitMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> DeathMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> ParriedMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	TObjectPtr<UAnimMontage> ComboAttackMontage;

	void SetBattleState(const BattleState NewState) { BattleState = NewState; }
	BattleState GetBattleState() const { return BattleState; }
	
protected:
	// 각종 동작에 필요한 상태값
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	BattleState BattleState = BattleState::None;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Equipment, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> WeaponMesh; 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> WeaponCollision;
	
	// Combat
public:
	UFUNCTION(BlueprintCallable, Category = Combat)
	virtual void ProcessComboCommand();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void SetParryable(bool Parryable) { IsParryable = Parryable; }

	bool GetIsInvincible() const { return IsInvincible; }
	bool GetIsParryable() const { return IsParryable; }

protected:
	UFUNCTION(BlueprintCallable, Category = Combat)
	virtual void Attack();

	// 콤보 관련 함수
	virtual void ComboActionBegin();
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	void ComboCheck();
	void SetComboCheckTimer();
	void ResetAttackTime();
	void AttackInputDelay();
	virtual void NotifyComboActionEnd();
	

	// 무적 타이밍에 활성화
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	bool IsInvincible = false;

	// 패리 가능한 공격 시에 활성화
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	bool IsParryable = false;

protected:
	// 히트 시작/종료 임플리멘테이션
	virtual void HitBegin_Implementation() override;
	virtual void HitEnd_Implementation() override;

	// Parry 엔드 시에 바인드할 델리게이트 함수
	virtual void ParriedEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

	// 콤보 체크 딜레이를 위한 조건 변수
	bool bCanAttack = true;

	// 현재 재생 중인 콤보 단계
	int32 CurrentCombo = 0;

	// 콤보에 사용할 데이터
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UComboActionData> ComboActionData;
	
	// 콤보 가능 여부를 판단하기 위한 타이머 핸들
	FTimerHandle ComboTimerHandle;
	FTimerHandle AttackInputDelayTimerHandle;

	// 콤보 체크 변수
	bool HasNextComboCommand = false;

	// Status
public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UDamageManager> DamageManager;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCharacterStatData> StatData;

	UFUNCTION(BlueprintCallable, Category = Status)
	float GetHPPercent();
	
	float WalkSpeed = 150.f;
	float RunSpeed = 300.f;
	
	UFUNCTION()
	void OnHPChanged(float NewHP);
	
	UFUNCTION()
	void Death();

	UFUNCTION(BlueprintImplementableEvent)
	void OnDeath();
	
	UFUNCTION(BlueprintCallable)
	void ChangeMoveSpeed(float Speed) const;
};
