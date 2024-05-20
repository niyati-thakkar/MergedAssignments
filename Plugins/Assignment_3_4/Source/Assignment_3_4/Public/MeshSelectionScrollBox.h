// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "MeshAssetManager.h"
#include "SMeshSelectionScrollBox.h"
#include "Widgets/SWidget.h"
#include "MeshSelectionScrollBox.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMeshAssetThumbnailSelectedEvent, const FMeshData&, MeshData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMaterialAssetThumbnailSelectedEvent, const FMaterialData&, MaterialData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTextureAssetThumbnailSelectedEvent, const FTextureData&, TextureData);

UCLASS()
class ASSIGNMENT_3_4_API UMeshSelectionScrollBox : public UWidget
{
	GENERATED_BODY()

protected:
	
	virtual TSharedRef<SWidget> RebuildWidget() override;

	virtual const FText GetPaletteCategory() override;

	UFUNCTION()
	void HandleMeshAssetThumbnailSelected(const FMeshData& MeshData);
	UFUNCTION()
	void HandleMaterialAssetThumbnailSelected(const FMaterialData& MeshData);
	UFUNCTION()
	void HandleTextureAssetThumbnailSelected(const FTextureData& MeshData);
private:
	TSharedPtr<SMeshSelectionScrollBox> ScrollableAssetBox;

public:
	//virtual TSharedRef<SWidget> RebuildWidget() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshData")
	UMeshAssetManager* MeshDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshData")
	bool IsFloorClicked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshData")
	float ThumbnailSize = 1;

	FMeshAssetThumbnailSelectedEvent OnMeshAssetThumbnailSelected;

	FMaterialAssetThumbnailSelectedEvent OnMaterialAssetThumbnailSelected;

	FTextureAssetThumbnailSelectedEvent OnTextureAssetThumbnailSelected;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mesh")
	AssetType AssetT;

	void SynchronizeProperties() override;
};
