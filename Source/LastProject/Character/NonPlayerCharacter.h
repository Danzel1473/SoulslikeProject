// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "LastProject/Interface/CharacterAIInterface.h"
#include "NonPlayerCharacter.generated.h"

UENUM(BlueprintType)
enum class EnemyType : uint8
{
	Grunt,
	Boss
};

/**
 * 
 */
UCLASS()
class LASTPROJECT_API ANonPlayerCharacter : public ACharacterBase, public ICharacterAIInterface
{
	GENERATED_BODY()

public:
	ANonPlayerCharacter();
	virtual void BeginPlay() override;

	// Combat
public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	UFUNCTION()
	void OnWeaponOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> AttackMontage;

	virtual void ParriedEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded) override;

	// AI
public:
	virtual float GetAIAttackRange() override { return AIAttackRange; }
	virtual float GetAIDetectRange() override { return AIDetectRange; }
	virtual float GetAIPatrolRadius() override { return AIPatrolRange; }
	virtual float GetAITurnSpeed() override { return AITurnSpeed; }
	EnemyType GetEnemyType() const { return Type; }

	virtual void SetAIAttackDelegeate(const FAICharacterAttackFinished& InOnAttackFinished) override;
	virtual void AttackByAI() override;
	virtual void FoucsRandomMove() override;

	virtual void NotifyComboActionEnd() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = "true"))
	bool IsInBattle = false;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
	EnemyType Type = EnemyType::Grunt;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = "true"))
	float AIAttackRange = 150.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = "true"))
	float AIDetectRange = 1000.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = "true"))
	float AIPatrolRange = 500.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = "true"))
	float AITurnSpeed = 240.f;
	
	FAICharacterAttackFinished OnAttackFinished;
};

