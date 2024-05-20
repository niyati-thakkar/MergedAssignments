// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MeshAssetManager.h"
#include <ArchMeshActor.h>

#include "DisplayWidget.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "SelectionWidget.h"
#include "WallGenerator.h"
#include "EnhancedInputSubsystems.h"
#include "InteractiveArchController.generated.h"

/**
 * 
 */


DECLARE_DELEGATE_TwoParams(PrintLog, const FString& Message, const FLinearColor& ColorOpacity)


UCLASS()
class ASSIGNMENT_3_4_API AInteractiveArchController : public APlayerController
{
	GENERATED_BODY()


public:
	UFUNCTION()
	void SpawnMesh(const FMeshData& MeshData);

	UFUNCTION()
	void ApplyMaterial(const FMaterialData& MaterialData);

	UFUNCTION()
	void ApplyTexture(const FTextureData& TextureData);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	UMeshAssetManager* MeshDataAsset;

	UPROPERTY()
	FVector LastHitLocation;

	UPROPERTY()
	AArchMeshActor* ArchMeshActor;

	 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<USelectionWidget> SelectionWidgetClass;

	UPROPERTY()
	USelectionWidget* SelectionWidget;

	void SetupInputComponent() override;


	UFUNCTION()
	void ToggleController(const FInputActionValue& InputAction);

	UFUNCTION()
	void GetMouseClick(const FInputActionValue& InputAction);

	void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void HideUI();

	UFUNCTION(BlueprintCallable)
	void DestroyWallMesh();

	UFUNCTION(BlueprintCallable)
	void DestroyWallComponent();

	UFUNCTION(BlueprintCallable)
	void DestroyAllWalls();

	UFUNCTION(BlueprintCallable)
	void GenerateNewWallComponent();

	UPROPERTY()
	AWallGenerator* UserWall;

	PrintLog DelegateLog;

	UFUNCTION()
	void LogMessage(const FString& Message, const FLinearColor& ColorOpacity);


	UPROPERTY()
	bool bIsWallGenerator = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UDisplayWidget> DisplayWidgetClass;

	UPROPERTY(BlueprintReadOnly)
	UDisplayWidget* WallWidget;

	int32 Index;
};
