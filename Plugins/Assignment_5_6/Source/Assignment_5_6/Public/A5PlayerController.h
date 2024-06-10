// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "MeshGenerator.h"
#include "SelectionArea.h"
#include "Blueprint/UserWidget.h"
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
	void GetMouseClick(const FInputActionValue& InputAction);
	void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateProgressBar(float Progress);

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void HideProgressPanel();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ShowProgressPanel();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UUserWidget* W_MeshGenerationUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> WidgetBPClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ASelectionArea* SelectionArea;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AMeshGenerator* MeshGenerator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isGenerating;
};
