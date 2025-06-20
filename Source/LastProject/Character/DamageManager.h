// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DamageManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHitAttack, AActor*, Attacker, AActor*, Hit, float, Damage);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LASTPROJECT_API UDamageManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UDamageManager();
	
protected:	
	// Sets default values for this component's properties
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintAssignable)
	FOnHitAttack OnHitAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCharacterStatData> StatData;

	// HP
public:
	void SetMaxHealth(const int32 HealthStat);
	void SetCurrentHealth(const int32 Health);
	
	void HitAttack(int32 Damage);

protected:
	UPROPERTY(EditAnywhere)
	int32 MaxHealth;
	
	UPROPERTY(EditAnywhere)
	int32 CurrentHealth;

	// Stamina
public:
	UFUNCTION(BlueprintCallable)
	void SetMaxStamina(const int32 StaminaStat);
	void SetCurrentStamina(const int32 Stamina);
	
protected:
	UPROPERTY(EditAnywhere)
	int32 MaxStamina;
	
	UPROPERTY(EditAnywhere)
	int32 CurrentStamina;
};
