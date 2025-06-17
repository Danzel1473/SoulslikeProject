// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponData.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

UCLASS()
class LASTPROJECT_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

	void InitializeWeapon(UWeaponData* newWeaponData);
	void AsyncLoadMesh();
	
protected:
	// 무기를 보여줄 메시 컴포넌트.
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	TObjectPtr<class USkeletalMeshComponent> Mesh;
	
	// 액터의 충돌을 담당할 박스 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	TObjectPtr<class UBoxComponent> Collision;

	float AttackPower;
	float Weight;

protected:
	UPROPERTY()
	UWeaponData* WeaponData;
};
