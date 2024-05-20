// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DisplayWidget.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENT_3_4_API UDisplayWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString String_Message;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor Color_Opacity;
};
