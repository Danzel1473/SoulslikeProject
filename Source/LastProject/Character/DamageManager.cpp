// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageManager.h"

#include "CharacterStatData.h"


void UDamageManager::SetStatusFromData(UCharacterStatData* StatData)
{
	if (StatData)
	{
		SetMaxHealth(StatData->HPStat);
		SetCurrentHealth(MaxHealth);
		SetMaxStamina(StatData->StaminaStat);
		SetCurrentStamina(MaxStamina);
	}
}

void UDamageManager::SetMaxHealth(const float HealthStat)
{
	if (HealthStat)
	{
		MaxHealth = HealthStat;
	}
}

void UDamageManager::SetCurrentHealth(const float Health)
{
	CurrentHealth = Health;
	OnHPChanged.Broadcast(CurrentHealth);
}

void UDamageManager::SetMaxStamina(const float StaminaStat)
{
	MaxStamina = StaminaStat;
}

void UDamageManager::SetCurrentStamina(const float Stamina)
{
	CurrentStamina = Stamina;
}

void UDamageManager::HitAttack(const float Damage)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0f, MaxHealth);
	OnHPChanged.Broadcast(CurrentHealth);
}

float UDamageManager::GetHPPercent() const
{
	if (MaxHealth > 0.0f)
	{
		return FMath::Clamp(static_cast<float>(CurrentHealth) / static_cast<float>(MaxHealth), 0.0f, 1.f);
	}
	return 0.0f;
}


