// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LPWeapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Sword,
	Shield,
	Spear
};

UCLASS()
class LASTPROJECT_API ALPWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties

	ALPWeapon();

	FORCEINLINE class UBoxComponent* GetTrigger() const { return Trigger; }
	
	// UFUNCTION()
	// void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
	// 	AActor* OtherActor,
	// 	UPrimitiveComponent* OtherComp,
	// 	int32 OtherBodyIndex,
	// 	bool bFromSweep,
	// 	const FHitResult& SweepResult);

protected:
	// 액터의 충돌을 담당할 박스 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	TObjectPtr<class UBoxComponent> Trigger;

	// 무기를 보여줄 메시 컴포넌트.
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnyWhere, Category = Weapon)
	TObjectPtr<class UWeaponData> Weapon;
};
