// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomGameModeBase.h"
#include "CustomGameStateBase.h"
#include "CustomPlayerController.h"

ACustomGameModeBase::ACustomGameModeBase()
{
	GameStateClass = ACustomGameStateBase::StaticClass();
	static ConstructorHelpers::FObjectFinder<UBlueprint> PlayerControllerBP(TEXT("/Assignment_1_2/GameMode/DynamicPlayerController.DynamicPlayerController"));
	if (PlayerControllerBP.Succeeded())
	{
		PlayerControllerClass = (UClass*)PlayerControllerBP.Object->GeneratedClass;
	}
	DefaultPawnClass = NULL;
}
