// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageManager.h"

// Sets default values for this component's properties
UDamageManager::UDamageManager()
{
}

void UDamageManager::HitAttack(AActor* Attacker, float Damage)
{
	HealthPoint -= Damage;
}


// Called when the game starts
void UDamageManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

