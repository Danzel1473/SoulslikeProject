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
	PrimaryComponentTick.bCanEverTick = true;
}

void ULockonComponent::TickComponent(float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bIsLockOnMode && CurrentTarget)
    {
        AActor* Owner = GetOwner();
        APlayerController* PlayerController = Cast<APlayerController>(Owner->GetInstigatorController());
        if (!PlayerController) return;

        FVector ToTarget = CurrentTarget->GetActorLocation() - Owner->GetActorLocation();
        FRotator TargetRot = ToTarget.Rotation();
        TargetRot.Pitch = 0.f;
        TargetRot.Roll = 0.f;
        
        PlayerController->SetControlRotation(FMath::RInterpTo(PlayerController->GetControlRotation(), TargetRot, DeltaTime, 16.f));
    }
}


void ULockonComponent::SortCandidatesLeftToRight()
{
    if (Candidates.Num() == 0) return;

    APlayerController* PlayerController = Cast<APlayerController>(GetOwner()->GetInstigatorController());
    if (!PlayerController) return;

    Candidates.Sort([PlayerController](const AActor& A, const AActor& B)
    {
        FVector2D ScreenPosA, ScreenPosB;
        bool bAOnScreen = PlayerController->ProjectWorldLocationToScreen(A.GetActorLocation(), ScreenPosA);
        bool bBOnScreen = PlayerController->ProjectWorldLocationToScreen(B.GetActorLocation(), ScreenPosB);

        if (bAOnScreen && bBOnScreen)
        {
            return ScreenPosA.X < ScreenPosB.X; // X좌표 기준 오름차순 (좌->우)
        }
        else if (bAOnScreen)
        {
            return true;  // A는 화면 안, B는 화면 밖 -> A가 앞
        }
        else if (bBOnScreen)
        {
            return false; // B는 화면 안, A는 화면 밖 -> B가 앞
        }

        return false;
    });
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

    Candidates.Empty();

    for (const FOverlapResult& Result : Overlaps)
    {
        AActor* Target = Result.GetActor();
        if (!Target || Target == Owner) continue;

        // 시야 중심 거리 계산
        FVector2D ScreenPos;
        if (PlayerController->ProjectWorldLocationToScreen(Target->GetActorLocation(), ScreenPos))
        {

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
                // 락온 대상 배열에 등록
                Candidates.Add(Target);

                // 거리 계산
                float DistToCenter = FVector2D::Distance(ScreenPos, ScreenCenter);
                if (DistToCenter < ClosestScreenDist)
                {
                    ClosestScreenDist = DistToCenter;
                    ClosestTarget = Target;
                }

                // Debug: 시야 내 적에게 디버그 스피어 그려줌
                DrawDebugSphere(GetWorld(), Target->GetActorLocation(), 30.f, 12, FColor::Red, false, 1.0f);
            }
        }
    }

    // 카메라 중심에서 가장 가까운 적을 락온 대상으로 설정
    CurrentTarget = ClosestTarget;
    SortCandidatesLeftToRight();

    if (CurrentTarget)
    {
        UE_LOG(LogTemp, Log, TEXT("락온 대상: %s"), *CurrentTarget->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("락온 대상 없음"));
    }
}

void ULockonComponent::ToggleLockOn()
{
    APlayerController* PlayerController = Cast<APlayerController>(GetOwner()->GetInstigatorController());

    if (bIsLockOnMode)
    {
        bIsLockOnMode = false;
        CurrentTarget = nullptr;
        PlayerController->SetIgnoreLookInput(false);

    }
    else
    {
        DetectLockOnTarget();
        if (CurrentTarget)
        {
            bIsLockOnMode = true;
            if (PlayerController)
                PlayerController->SetIgnoreLookInput(true);
        }
    }
}

void ULockonComponent::SwitchTarget(bool bRight)
{
    if (!bIsLockOnMode || !CurrentTarget) return;

    AActor* Owner = GetOwner();
    SortCandidatesLeftToRight();

    APlayerController* PlayerController = Cast<APlayerController>(Owner->GetInstigatorController());
    if (!PlayerController) return;
    int32 CurrentTargetNum = Candidates.Find(CurrentTarget);
    
    if (bRight)
    {
        if (CurrentTargetNum != Candidates.Num() - 1)
            CurrentTarget = Candidates[CurrentTargetNum + 1];
        else
        {
            CurrentTarget = Candidates[0];
        }
    }
    else
    {
        if (CurrentTargetNum != 0)
            CurrentTarget = Candidates[CurrentTargetNum - 1];
        else
        {
            CurrentTarget = Candidates[Candidates.Num() - 1];
        }
    }
}
