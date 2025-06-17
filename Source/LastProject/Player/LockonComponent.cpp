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
    SetLockOnMode(false);
}

void ULockonComponent::BeginPlay()
{
    Super::BeginPlay();

    Owner = GetOwner();
    PlayerController = Cast<APlayerController>(Owner->GetInstigatorController());

    OnLockOnModeChanged.AddDynamic(this, &ULockonComponent::LockOnModeChanged);

}

void ULockonComponent::SortCandidatesLeftToRight()
{
    if (Candidates.Num() == 0) return;

    if (!PlayerController) return;

    Candidates.Sort([this](const AActor& A, const AActor& B)
    {
        if (!PlayerController) return false;
        
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

void ULockonComponent::LockOnModeChanged(bool IsLockOn)
{
    if (IsLockOn)
    {
        // 주기적으로 락온 대상을 체크할 타이머 스타트
        UE_LOG(LogInput, Log, TEXT("타이머 스타트"));
        GetWorld()->GetTimerManager().SetTimer(LockOnCheckTimer, [this]()
            {
                DetectLockOnTarget();
                SortCandidatesLeftToRight();

                // 락온 대상 배열이 없다면 락온 모드 해제
                if (Candidates.Num() < 1)
                {
                    SetLockOnMode(false);
                }
                // 락온 대상 배열이 비어 있지 않은데, CurrentTarget이 Candidates에 없을 경우
                else if (!Candidates.Contains(CurrentTarget))
                {
                    CurrentTarget = FindBestTargetFromCandidates();
                }
            }, 0.6f, true);
    }
    else
    {
        UE_LOG(LogInput, Log, TEXT("타이머 종료"));

        GetWorld()->GetTimerManager().ClearTimer(LockOnCheckTimer);
    }
}

void ULockonComponent::DetectLockOnTarget()
{
    if (!Owner) return;
    if (!PlayerController) return;

    FVector CameraLoc;
    FRotator CameraRot;
    PlayerController->GetPlayerViewPoint(CameraLoc, CameraRot);

    // 구체 탐색 (적 후보 수집)
    TArray<FOverlapResult> Overlaps;
    FCollisionQueryParams Params(NAME_None, false, Owner);
    
    UE_LOG(LogInput, Log, TEXT("락온 대상 스캔"));

    // Enemy 채널로 1차 판별
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

            if (!bBlocked || Hit.GetActor() == Target || Hit.GetActor()->ActorHasTag("Enemy"))
            {
                // 락온 대상 배열에 등록
                Candidates.Add(Target);

                // Debug: 시야 내 적에게 디버그 스피어 그려줌
                DrawDebugSphere(GetWorld(), Target->GetActorLocation(), 30.f, 12, FColor::Red, false, 1.0f);
            }
        }
    }

    if (Candidates.Num() <= 0)
    {
        if (bIsLockOnMode)
        {
            SetLockOnMode(false);
        }
    }

    if (CurrentTarget)
    {
        UE_LOG(LogTemp, Log, TEXT("락온 대상: %s"), *CurrentTarget->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("락온 대상 없음"));
    }
}

AActor* ULockonComponent::FindBestTargetFromCandidates()
{
    if (!PlayerController || Candidates.Num() == 0) return nullptr;

    int32 SizeX, SizeY;
    PlayerController->GetViewportSize(SizeX, SizeY);
    FVector2D ScreenCenter(SizeX * 0.5f, SizeY * 0.5f);

    float ClosestDist = FLT_MAX;
    AActor* BestTarget = nullptr;

    for (AActor* Target : Candidates)
    {
        if (!Target) continue;

        FVector2D ScreenPos;
        if (PlayerController->ProjectWorldLocationToScreen(Target->GetActorLocation(), ScreenPos))
        {
            // 거리 계산
            float Dist = FVector2D::Distance(ScreenPos, ScreenCenter);
            if (Dist < ClosestDist)
            {
                ClosestDist = Dist;
                BestTarget = Target;
            }
        }
    }

    return BestTarget;
}


void ULockonComponent::ToggleLockOn()
{
    if (bIsLockOnMode)
    {
        SetLockOnMode(false);
        CurrentTarget = nullptr;
    }
    else
    {
        // 락온배열 탐지
        DetectLockOnTarget();
        if (Candidates.Num() > 0)
        {
            // 카메라 중심에서 가장 가까운 적을 락온 대상으로 설정
            CurrentTarget = FindBestTargetFromCandidates();
            // 락온배열 정렬
            SortCandidatesLeftToRight();
        }
        
        if (CurrentTarget)
        {
            SetLockOnMode(true);
            
        }
    }
}

void ULockonComponent::SwitchTarget(bool bRight)
{
    if (!bIsLockOnMode || !CurrentTarget) return;

    DetectLockOnTarget();
    SortCandidatesLeftToRight();

    if (!PlayerController) return;
    int32 CurrentTargetNum = Candidates.Find(CurrentTarget);
    
    if (bRight)
    {
        if (Candidates.Num() <= 1 ) return;

        if (CurrentTargetNum < 0)
        {
            CurrentTarget = Candidates[Candidates.Num() - 1];
            return;
        }
        
        if (CurrentTargetNum != Candidates.Num() - 1)
            CurrentTarget = Candidates[CurrentTargetNum + 1];
        else
        {
            CurrentTarget = Candidates[0];
        }
    }
    else
    {
        if (Candidates.Num() <= 1 ) return;
        if (CurrentTargetNum < 0)
        {
            CurrentTarget = Candidates[0];
            return;
        }

        UE_LOG(LogInput, Log, TEXT("CurrentTargetNum = %d"), CurrentTargetNum);

        if (CurrentTargetNum != 0)
            CurrentTarget = Candidates[CurrentTargetNum - 1];
        else
        {
            CurrentTarget = Candidates[Candidates.Num() - 1];
        }
    }
}

void ULockonComponent::SetLockOnMode(bool IsLockOn)
{
    if (bIsLockOnMode == IsLockOn) return;
    
    bIsLockOnMode = IsLockOn;
    OnLockOnModeChanged.Broadcast(bIsLockOnMode);
}
