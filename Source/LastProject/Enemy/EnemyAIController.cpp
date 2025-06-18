// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"

#include "TimerManager.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "LastProject/Character/CharacterBase.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AEnemyAIController::AEnemyAIController()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	SightConfig->SightRadius = 1000.f;
	SightConfig->LoseSightRadius = 1200.f;
	SightConfig->PeripheralVisionAngleDegrees = 60.f;

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	PerceptionComponent->ConfigureSense(*SightConfig);
	PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetPerceptionUpdated);
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
	}

	if (GetBlackboardComponent() && GetPawn())
	{
		GetBlackboardComponent()->SetValueAsVector("HomeLocation", GetPawn()->GetActorLocation());
	}
}

void AEnemyAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		if (GetBlackboardComponent())
		{
			// 탐지 액터 조건 주기
			GetBlackboardComponent()->SetValueAsObject("TargetObject", Actor);
		}
		// 기존 타이머 제거
		GetWorld()->GetTimerManager().ClearTimer(LoseSightTimerHandle);
		GetWorld()->GetTimerManager().ClearTimer(TargetLocationUpdateTimer);
		
		// 위치 갱신용 타이머 등록
		GetWorld()->GetTimerManager().SetTimer(TargetLocationUpdateTimer, [this]()
			{
				if (!GetBlackboardComponent()) return;

				AActor* Target = Cast<AActor>(GetBlackboardComponent()->GetValueAsObject("TargetObject"));
				if (Target)
				{
					GetBlackboardComponent()->SetValueAsVector("LastTargetLocation", Target->GetActorLocation());
				}
			},
			0.15f,
			true
		);

		// 뛰는 속도로 변경
		ACharacterBase* OwnerChar = Cast<ACharacterBase>(GetPawn());
		if (OwnerChar)
		{
			OwnerChar->ChangeMoveSpeed(OwnerChar->RunSpeed);
		}
	}
	else
	{
		if (GetBlackboardComponent())
		{
			GetWorld()->GetTimerManager().SetTimer(LoseSightTimerHandle, [this]()
		{
			if (!GetBlackboardComponent()) return;
				
			AActor* TargetObject = Cast<AActor>(GetBlackboardComponent()->GetValueAsObject("TargetObject"));
			if (TargetObject)
			{
				GetBlackboardComponent()->SetValueAsObject("TargetObject", nullptr);
				GetWorld()->GetTimerManager().ClearTimer(TargetLocationUpdateTimer);

				// 걷는 속도로 변경
				ACharacterBase* OwnerChar = Cast<ACharacterBase>(GetPawn());
				if (OwnerChar)
				{
					OwnerChar->ChangeMoveSpeed(OwnerChar->WalkSpeed);
				}

			}
		}, 3.0f, false);
		}
	}
}
