// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageManager.h"

// Sets default values for this component's properties
UDamageManager::UDamageManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
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

