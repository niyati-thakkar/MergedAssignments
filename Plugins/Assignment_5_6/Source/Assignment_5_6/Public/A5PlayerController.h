// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"

#include "A5PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENT_5_6_API AA5PlayerController : public APlayerController
{
	GENERATED_BODY()


protected:
	void SetupInputComponent() override;

public:
	

};
