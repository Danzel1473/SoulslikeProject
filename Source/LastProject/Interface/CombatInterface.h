// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LASTPROJECT_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	void EnableWeaponCollision();

	UFUNCTION(BlueprintNativeEvent)
	void DisableWeaponCollision();
	
	UFUNCTION(BlueprintNativeEvent)
	void HitBegin();

	UFUNCTION(BlueprintNativeEvent)
	void HitEnd();
};