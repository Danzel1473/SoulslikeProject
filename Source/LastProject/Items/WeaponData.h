// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "../../../../../../../Program Files/Epic Games/UE_5.4/Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Animation/AnimMontage.h"
#include "Engine/SkeletalMesh.h"
#include "WeaponData.generated.h"

/**
 * 
 */
UCLASS()
class LASTPROJECT_API UWeaponData : public UItemData
{
	GENERATED_BODY()

public:
	UWeaponData();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<USkeletalMesh> WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	FVector CollisionBoxExtent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	FVector CollisionBoxOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	float AttackPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	float Weight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AttackAnimation;
};