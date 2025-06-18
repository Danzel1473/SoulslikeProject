// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "NonPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class LASTPROJECT_API ANonPlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	ANonPlayerCharacter();
	virtual void BeginPlay() override;

	// Combat
public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
protected:
	// virtual void Attack() override;
	// void AttackEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> AttackMontage;
};
