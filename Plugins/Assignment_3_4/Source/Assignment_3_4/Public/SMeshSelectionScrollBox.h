// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Layout/SScrollBox.h"
#include "MeshAssetManager.h"
#include "SlateOptMacros.h"
#include "MeshAssetManager.h"
#include "Styling/SlateBrush.h"
#include "Engine/Texture2D.h"
#include "Widgets/Input/SEditableTextBox.h"

/**
 * 
 */
DECLARE_DELEGATE_OneParam(FAssetTextureThumbnailSelected, const FTextureData&);
DECLARE_DELEGATE_OneParam(FAssetMaterialThumbnailSelected, const FMaterialData&);
DECLARE_DELEGATE_OneParam(FAssetMeshThumbnailSelected, const FMeshData&);

class ASSIGNMENT_3_4_API SMeshSelectionScrollBox : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMeshSelectionScrollBox)
	{}
	SLATE_ARGUMENT(TWeakObjectPtr<UMeshAssetManager>, InMeshDataAsset)
	SLATE_ARGUMENT(float, InThumbnailSize)
	SLATE_END_ARGS()


private:
	TSharedPtr<SScrollBox> ScrollBox;
	
	TSharedPtr<SHorizontalBox> HorizontalBox;
	TWeakObjectPtr<UMeshAssetManager> MeshDataAsset;
	UObject* MeshComponent;

public:
	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	float ThumbnailSize;

	void RefreshContent();

	void DisplayMeshes(TArray<FMeshData> DataArray);
	void DisplayMaterials(TArray<FMaterialData> DataArray);
	void DisplayTextures(TArray<FTextureData> DataArray);

	FAssetTextureThumbnailSelected OnTextureThumbnailSelected;
	FAssetMaterialThumbnailSelected OnMaterialThumbnailSelected;
	FAssetMeshThumbnailSelected OnMeshThumbnailSelected;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	AssetType AssetT;
};
