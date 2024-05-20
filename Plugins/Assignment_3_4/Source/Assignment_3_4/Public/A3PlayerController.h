// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "WallGenerator.h"
#include "A3PlayerController.generated.h"

/**
 * 
 */

DECLARE_DELEGATE_TwoParams(PrintLog,const FString& Message, const FLinearColor& ColorOpacity)

UCLASS()
class ASSIGNMENT_3_4_API AA3PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AA3PlayerController();

	UPROPERTY()
	AWallGenerator* UserWall;

	void BeginPlay();
protected:
	void SetupInputComponent() override;
	void GetLocation();

public:
	PrintLog DelegateLog;

	UFUNCTION(BlueprintImplementableEvent)
	void LogMessage(const FString& Message, const FLinearColor& ColorOpacity);
	
	UFUNCTION(BlueprintCallable)
	void DestroyWallMesh();

	UFUNCTION(BlueprintCallable)
	void DestroyWallComponent();

	UFUNCTION(BlueprintCallable)
	void DestroyAllWalls();

	UFUNCTION(BlueprintCallable)
	void GenerateNewWallComponent();
};
