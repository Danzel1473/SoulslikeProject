// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterStatData.generated.h"

/**
 * 
 */
UCLASS()
class LASTPROJECT_API UCharacterStatData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	int32 HPStat = 100;
	
	UPROPERTY(EditAnywhere)
	int32 StaminaStat = 100;
	
	UPROPERTY(EditAnywhere)
	int32 PowerStat = 10;
};
