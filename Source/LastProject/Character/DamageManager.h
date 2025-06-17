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
	// Sets default values for this component's properties
	UDamageManager();

	UPROPERTY(BlueprintAssignable)
	FOnHitAttack OnHitAttack;

public:
	void HitAttack(AActor* Attacker, float Damage);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(EditAnywhere)
	float HealthPoint;

	UPROPERTY(EditAnywhere)
	float Stamina;
};
