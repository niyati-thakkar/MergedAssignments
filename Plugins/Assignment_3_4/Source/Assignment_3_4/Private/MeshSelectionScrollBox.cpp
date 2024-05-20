// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshSelectionScrollBox.h"

#define LOCTEXT_NAMESPACE "ScrollableAssetBoxWidget"

TSharedRef<SWidget> UMeshSelectionScrollBox::RebuildWidget()
{
	ScrollableAssetBox = SNew(SMeshSelectionScrollBox).InMeshDataAsset(MeshDataAsset).InThumbnailSize(ThumbnailSize);
	ScrollableAssetBox->OnMeshThumbnailSelected.BindUObject(this, &UMeshSelectionScrollBox::HandleMeshAssetThumbnailSelected);
	ScrollableAssetBox->OnMaterialThumbnailSelected.BindUObject(this, &UMeshSelectionScrollBox::HandleMaterialAssetThumbnailSelected);
	ScrollableAssetBox->OnTextureThumbnailSelected.BindUObject(this, &UMeshSelectionScrollBox::HandleTextureAssetThumbnailSelected);
	return ScrollableAssetBox.ToSharedRef();
}


const FText UMeshSelectionScrollBox::GetPaletteCategory()
{
	return LOCTEXT("PaletteCategory", "Panel");
}


void UMeshSelectionScrollBox::HandleMeshAssetThumbnailSelected(const FMeshData& MeshData) {
	OnMeshAssetThumbnailSelected.Broadcast(MeshData);
}

void UMeshSelectionScrollBox::HandleMaterialAssetThumbnailSelected(const FMaterialData& MaterialData)
{
	OnMaterialAssetThumbnailSelected.Broadcast(MaterialData);
}

void UMeshSelectionScrollBox::HandleTextureAssetThumbnailSelected(const FTextureData& TextureData)
{
	OnTextureAssetThumbnailSelected.Broadcast(TextureData);
}

void UMeshSelectionScrollBox::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	if (ScrollableAssetBox) {
		ScrollableAssetBox->ThumbnailSize = ThumbnailSize;
		ScrollableAssetBox->AssetT = AssetT;
		ScrollableAssetBox->RefreshContent();
	}
	
}

#undef LOCTEXT_NAMESPACE