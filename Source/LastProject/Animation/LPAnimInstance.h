// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "LPAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class LASTPROJECT_API ULPAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	ULPAnimInstance();
	
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	float CalculateDirection();
	
	// 프로퍼티.
public:
	// 애님 인스턴스를 소유하는 캐릭터 참조 변수.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class ACharacter> Owner;

	// 캐릭터 무브먼트 컴포넌트 참조 변수.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	// 캐릭터의 현재 이동 속도를 저장할 변수.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float Direction;	

	// 캐릭터의 땅에서의 이동 속도를 저장할 변수.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float GroundSpeed;

	// 캐릭터가 Idle인지 여부를 확인할 변수.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsIdle : 1;

	// 캐릭터가 이동하는지(멈춰있는지) 여부를 판단하는 데 사용할 문턱 값.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float MovingThreshold;

	// 캐릭터가 공중에서 떨어지는지 여부를 확인하는데 사용할 문턱 값.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsFalling : 1;

	// 캐릭터가 점프하는지를 검사할 수 있는 값.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsJumping : 1;
	
	// 캐릭터가 점프하는지를 검사할 수 있는 값.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsDead : 1;

	// 캐릭터가 점프하는지 여부를 판단하는 데 사용할 문턱 값.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float JumpingThreshold;
};
