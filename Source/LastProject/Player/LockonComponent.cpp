// Fill out your copyright notice in the Description page of Project Settings.


#include "LockonComponent.h"

#include "CollisionQueryParams.h"
#include "DrawDebugHelpers.h"
#include "Engine/HitResult.h"
#include "Engine/OverlapResult.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"

#define ECC_Enemy ECC_GameTraceChannel1

// Sets default values for this component's properties
ULockonComponent::ULockonComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void ULockonComponent::DetectLockOnTarget()
{
	AActor* Owner = GetOwner();
    if (!Owner) return;

    APlayerController* PlayerController = Cast<APlayerController>(Owner->GetInstigatorController());
    if (!PlayerController) return;

    FVector CameraLoc;
    FRotator CameraRot;
    PlayerController->GetPlayerViewPoint(CameraLoc, CameraRot);
    FVector CameraForward = CameraRot.Vector();

    // 구체 탐색 (적 후보 수집)
    TArray<FOverlapResult> Overlaps;
    FCollisionQueryParams Params(NAME_None, false, Owner);

    GetWorld()->OverlapMultiByObjectType(
        Overlaps,
        Owner->GetActorLocation(),
        FQuat::Identity,
        FCollisionObjectQueryParams(ECC_Enemy),
        FCollisionShape::MakeSphere(MaxDistance),
        Params
    );

    int32 SizeX, SizeY;
    PlayerController->GetViewportSize(SizeX, SizeY);

    FVector2D ScreenCenter(SizeX * 0.5f, SizeY * 0.5f);
    
    float ClosestScreenDist = FLT_MAX;
    AActor* ClosestTarget = nullptr;

    for (const FOverlapResult& Result : Overlaps)
    {
        AActor* Target = Result.GetActor();
        if (!Target || Target == Owner) continue;

        // 시야 중심 거리 계산
        FVector2D ScreenPos;
        if (PlayerController->ProjectWorldLocationToScreen(Target->GetActorLocation(), ScreenPos))
        {
            float DistToCenter = FVector2D::Distance(ScreenPos, ScreenCenter);

            // LineTrace로 시야 가림 여부 확인
            FHitResult Hit;
            bool bBlocked = GetWorld()->LineTraceSingleByChannel(
                Hit,
                CameraLoc,
                Target->GetActorLocation(),
                ECC_Visibility,
                Params
            );

            if (!bBlocked || Hit.GetActor() == Target)
            {
                if (DistToCenter < ClosestScreenDist)
                {
                    ClosestScreenDist = DistToCenter;
                    ClosestTarget = Target;
                }

                // 디버그용: 시야 내 적에게 디버그 스피어
                DrawDebugSphere(GetWorld(), Target->GetActorLocation(), 30.f, 12, FColor::Red, false, 1.0f);
            }
        }
    }

    CurrentTarget = ClosestTarget;

    if (CurrentTarget)
    {
        UE_LOG(LogTemp, Warning, TEXT("락온 대상: %s"), *CurrentTarget->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("락온 대상 없음"));
    }
}

void ULockonComponent::ToggleLockOn()
{
}

void ULockonComponent::SwitchTarget(bool bRight)
{
}
