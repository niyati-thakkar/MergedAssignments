// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MeshAssetManager.h"
#include "MeshSelectionScrollBox.h"
#include "SMeshSelectionScrollBox.h"
#include "SelectionWidget.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENT_3_4_API USelectionWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	void InitializeWidget(UMeshAssetManager* MeshDataAsset);
	//UFUNCTION(BlueprintAssignableEvent)
	UPROPERTY(BlueprintAssignable, Category = "Overlay Widget")
	FMeshAssetThumbnailSelectedEvent MeshThumbnailDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Overlay Widget")
	FMaterialAssetThumbnailSelectedEvent MaterialThumbnailDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Overlay Widget")
	FTextureAssetThumbnailSelectedEvent TextureThumbnailDelegate;

	UPROPERTY(BlueprintReadWrite, Category = "Overlay Widget", meta = (BindWidget))
	UMeshSelectionScrollBox* MeshScrollBox;

	UPROPERTY(BlueprintReadWrite, Category = "Overlay Widget", meta = (BindWidget))
	UMeshSelectionScrollBox* MaterialScrollBox;

	UPROPERTY(BlueprintReadWrite, Category = "Overlay Widget", meta = (BindWidget))
	UMeshSelectionScrollBox* TextureScrollBox;


	UFUNCTION()
	void ReceiveMeshData(const FMeshData& MeshData);
	UFUNCTION()
	void ReceiveMaterialData(const FMaterialData& MaterialData);
	UFUNCTION()
	void ReceiveTextureData(const FTextureData& TexureData);



};
