// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	
	SetRootComponent(Mesh.Get());
	Collision->SetupAttachment(RootComponent);
}

void AWeaponBase::InitializeWeapon(UWeaponData* NewWeaponData)
{
	if (NewWeaponData)
	{
		WeaponData = NewWeaponData;
	}

	if (WeaponData->WeaponMesh.IsValid())
	{
		Mesh->SetSkeletalMesh(WeaponData->WeaponMesh.LoadSynchronous());
	}

	Collision->SetBoxExtent(WeaponData->CollisionBoxExtent);
	Collision->SetRelativeLocation(WeaponData->CollisionBoxOffset);

	AttackPower = WeaponData->AttackPoint;
}

