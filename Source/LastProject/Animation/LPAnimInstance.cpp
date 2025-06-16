// Fill out your copyright notice in the Description page of Project Settings.


#include "LPAnimInstance.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

ULPAnimInstance::ULPAnimInstance()
{
	// 문턱 값 설정.
	MovingThreshold = 3.0f;
	JumpingThreshold = 100.0f;
}

void ULPAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void ULPAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (Movement)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();
		bIsIdle = GroundSpeed < MovingThreshold;
		bIsFalling = Movement->IsFalling();
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshold);
		Direction = CalculateDirection();
	}
}

float ULPAnimInstance::CalculateDirection()
{
	if (Owner)
	{
		if (Velocity.IsNearlyZero()) return 0.f;

		FVector Forward = Owner->GetActorForwardVector();
		FVector MoveDir = Velocity.GetSafeNormal();

		float AngleRad = FMath::Acos(FVector::DotProduct(Forward, MoveDir));
		float AngleDeg = FMath::RadiansToDegrees(AngleRad);

		// 왼쪽/오른쪽 판별
		float Sign = FVector::CrossProduct(Forward, MoveDir).Z < 0 ? -1.f : 1.f;

		return AngleDeg * Sign; // -180 ~ 180 범위
	}
	return 0.f;
}
