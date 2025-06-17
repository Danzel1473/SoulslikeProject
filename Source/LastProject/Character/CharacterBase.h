// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "LastProject/Interface/CombatInterface.h"
#include "CharacterBase.generated.h"

UENUM(BlueprintType)
enum class BattleState : uint8
{
	None = 0	UMETA(DisplayName = "None"),
	Attacking	UMETA(DisplayName = "Attacking"),
	Guarding	UMETA(DisplayName = "Guarding"),
	Dodging		UMETA(DisplayName = "Dodging"),
};

UCLASS()
class LASTPROJECT_API ACharacterBase : public ACharacter, public ICombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

	void BeginPlay() override;

	void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;	
	virtual void SetCharacterControlData(const class UCharacterControlData* InCharacterControlData);

	virtual void EnableWeaponCollision_Implementation() override;
	virtual void DisableWeaponCollision_Implementation() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	BattleState BattleState;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Equipment, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> WeaponMesh; 
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> WeaponCollision;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class AWeaponBase> Weapon;
public:
	
};
