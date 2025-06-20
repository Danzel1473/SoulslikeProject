// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DamageManager.generated.h"

// DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHitAttack, AActor*, Attacker, AActor*, Hit, float, Damage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHPChanged, float, CurrentHP);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LASTPROJECT_API UDamageManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetStatusFromData(class UCharacterStatData* StatData);

	UPROPERTY(BlueprintAssignable, Category = Stat)
	FOnHPChanged OnHPChanged;
	
	
	// HP
public:
	void SetMaxHealth(const float HealthStat);
	void SetCurrentHealth(const float Health);
	
	void HitAttack(float Damage);

	UFUNCTION(BlueprintCallable)
	float GetHPPercent() const;

protected:
	UPROPERTY(EditAnywhere)
	float MaxHealth;
	
	UPROPERTY(EditAnywhere)
	float CurrentHealth;

	// Stamina
public:
	UFUNCTION(BlueprintCallable)
	void SetMaxStamina(const float StaminaStat);
	void SetCurrentStamina(const float Stamina);
	
protected:
	UPROPERTY(EditAnywhere)
	float MaxStamina;
	
	UPROPERTY(EditAnywhere)
	float CurrentStamina;
};
