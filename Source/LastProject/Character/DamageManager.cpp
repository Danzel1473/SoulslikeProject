// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageManager.h"

#include "CharacterStatData.h"

// Sets default values for this component's properties
UDamageManager::UDamageManager()
{
}

// Called when the game starts
void UDamageManager::BeginPlay()
{
	if (StatData)
	{
		SetMaxHealth(StatData->HPStat);
		SetCurrentHealth(MaxHealth);
		SetMaxStamina(StatData->StaminaStat);
		SetCurrentStamina(MaxStamina);
	}
}

void UDamageManager::SetMaxHealth(const int32 HealthStat)
{
	if (HealthStat)
	{
		MaxHealth = HealthStat;
	}
}

void UDamageManager::SetCurrentHealth(const int32 Health)
{
	CurrentHealth = Health;
}

void UDamageManager::SetMaxStamina(const int32 StaminaStat)
{
	MaxStamina = StaminaStat;
}

void UDamageManager::SetCurrentStamina(const int32 Stamina)
{
	CurrentStamina = Stamina;
}

void UDamageManager::HitAttack(const int32 Damage)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0, MaxHealth);
}


