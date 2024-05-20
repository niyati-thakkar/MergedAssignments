// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectionWidget.h"

#include "SelectionWidget.h"

void USelectionWidget::InitializeWidget(UMeshAssetManager* MeshDataAsset)
{
	MeshScrollBox->MeshDataAsset = MeshDataAsset;
	MaterialScrollBox->MeshDataAsset = MeshDataAsset;
	TextureScrollBox->MeshDataAsset = MeshDataAsset;

	MeshScrollBox->OnMeshAssetThumbnailSelected.AddDynamic(this, &USelectionWidget::ReceiveMeshData);
	MeshScrollBox->OnMaterialAssetThumbnailSelected.AddDynamic(this, &USelectionWidget::ReceiveMaterialData);
	MeshScrollBox->OnTextureAssetThumbnailSelected.AddDynamic(this, &USelectionWidget::ReceiveTextureData);

	MaterialScrollBox->OnMeshAssetThumbnailSelected.AddDynamic(this, &USelectionWidget::ReceiveMeshData);
	MaterialScrollBox->OnMaterialAssetThumbnailSelected.AddDynamic(this, &USelectionWidget::ReceiveMaterialData);
	MaterialScrollBox->OnTextureAssetThumbnailSelected.AddDynamic(this, &USelectionWidget::ReceiveTextureData);

	TextureScrollBox->OnMeshAssetThumbnailSelected.AddDynamic(this, &USelectionWidget::ReceiveMeshData);
	TextureScrollBox->OnMaterialAssetThumbnailSelected.AddDynamic(this, &USelectionWidget::ReceiveMaterialData);
	TextureScrollBox->OnTextureAssetThumbnailSelected.AddDynamic(this, &USelectionWidget::ReceiveTextureData);
}




void USelectionWidget::ReceiveMeshData(const FMeshData& MeshData)
{
	MeshThumbnailDelegate.Broadcast(MeshData);
}

void USelectionWidget::ReceiveMaterialData(const FMaterialData& MaterialData)
{
	MaterialThumbnailDelegate.Broadcast(MaterialData);
}

void USelectionWidget::ReceiveTextureData(const FTextureData& TextureData)
{
	TextureThumbnailDelegate.Broadcast(TextureData);
}


