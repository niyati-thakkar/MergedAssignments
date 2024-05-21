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
#include "Engine/DataTable.h"
#include "InteractiveArchController.generated.h"

/**
 * 
 */


DECLARE_DELEGATE_TwoParams(PrintLog, const FString& Message, const FLinearColor& ColorOpacity)

UENUM()
enum class EViewPawnType : uint8 {
	PerspectiveView UMETA(DisplayName = "Perspective View"),
	IsometricView UMETA(DisplayName = "Isometric View"),
	OrthographicView UMETA(DisplayName = "Orthographic View")
};

USTRUCT(BlueprintType)
struct FViewPawnDataTable : public FTableRowBase {

	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EViewPawnType PawnType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<APawn> PawnClass;

};
UCLASS()
class ASSIGNMENT_3_4_API AInteractiveArchController : public APlayerController
{
	GENERATED_BODY()


public:

	AInteractiveArchController();
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


	UFUNCTION(BlueprintCallable)
	void ToggleController();

	UFUNCTION()
	void GetMouseClick(const FInputActionValue& InputAction);
	void SpawnActors(FHitResult HitResult);
	void GenerateWall(FHitResult HitResult);

	void BeginPlay() override;
	void AddMappings();

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


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	class UDataTable* PawnTypeDataTable{};

	void SetEnhancedInputToggle();

	UFUNCTION(BlueprintCallable)
	void SpawnPawn();
	UPROPERTY(EditAnywhere, BlueprintReadWrite);


	int32 Index;

	APawn* CurrentPawn;
	FVector CurrentPawnLocation;
	FRotator CurrentPawnRotation;
	TArray<FViewPawnDataTable*> TypesOfPawns;
};
