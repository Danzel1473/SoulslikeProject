// Fill out your copyright notice in the Description page of Project Settings.


#include "LPPlayerController.h"

void ALPPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameInput;
	SetInputMode(GameInput);
}
