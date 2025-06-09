// Fill out your copyright notice in the Description page of Project Settings.


#include "LPGameMode.h"

ALPGameMode::ALPGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> DefaultCharacterRef(
		TEXT("/Game/LastProject/Character/BP_Player.BP_Player_C"));

	if (DefaultCharacterRef.Class)
	{
		DefaultPawnClass = DefaultCharacterRef.Class;
	}

	// PlayerControllerClass.
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/LastProject.LPPlayerController"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
}