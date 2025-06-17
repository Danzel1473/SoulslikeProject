// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"

#include "CharacterControlData.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "LastProject/LastProject.h"
#include "LastProject/Items/WeaponBase.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 회전 제어
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 400.0f;

	GetCapsuleComponent()->SetCapsuleHalfHeight(88.0f);

	BattleState = BattleState::None;

	// 
	GetMesh()->SetRelativeLocationAndRotation(
		FVector(0.0f, 0.0f, -88.0f),
		FRotator(0.0f, -90.0f, 0.0f)
	);

	FName SocketName = TEXT("RightHandSocket");

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(GetMesh(), SocketName);

	WeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollision"));
	WeaponCollision->SetupAttachment(WeaponMesh);
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float ACharacterBase::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


void ACharacterBase::SetCharacterControlData(const class UCharacterControlData* InCharacterControlData)
{
	// Pawn.
	bUseControllerRotationYaw = InCharacterControlData->bUseControllerRotationYaw;

	// CharacterMovement.
	GetCharacterMovement()->bOrientRotationToMovement = InCharacterControlData->bOrientRotationToMovement;
	GetCharacterMovement()->bUseControllerDesiredRotation = InCharacterControlData->bUseControllerDesiredRotation;
	GetCharacterMovement()->RotationRate = InCharacterControlData->RotationRate;
}

void ACharacterBase::EnableWeaponCollision_Implementation()
{
	ICombatInterface::EnableWeaponCollision_Implementation();

	if (WeaponCollision)
	{
		WeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		UE_LOG(LogInput, Log, TEXT("WeaponCollision Status: %s"), WeaponCollision->GetCollisionEnabled() ? TEXT("Enabled") : TEXT("Disabled"));
	}
}

void ACharacterBase::DisableWeaponCollision_Implementation()
{
	ICombatInterface::DisableWeaponCollision_Implementation();
	
	if (WeaponCollision)
	{
		WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		UE_LOG(LogInput, Log, TEXT("WeaponCollision Status: %s"), WeaponCollision->GetCollisionEnabled() ? TEXT("Enabled") : TEXT("Disabled"));
	}
}

