// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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
class LASTPROJECT_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

	void BeginPlay() override;

	void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;	
	virtual void SetCharacterControlData(const class UCharacterControlData* InCharacterControlData);
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	BattleState BattleState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class AWeaponBase> Weapon;
public:
	
};
