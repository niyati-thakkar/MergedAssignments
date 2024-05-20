// Fill out your copyright notice in the Description page of Project Settings.


#include "SMeshSelectionScrollBox.h"




BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SMeshSelectionScrollBox::Construct(const FArguments& InArgs)
{
	MeshDataAsset = InArgs._InMeshDataAsset;
	ThumbnailSize = InArgs._InThumbnailSize;

	HorizontalBox = SNew(SHorizontalBox);
	ScrollBox = SNew(SScrollBox).Orientation(Orient_Horizontal);
	// Set the horizontal box as child slot
	ChildSlot
		[
			HorizontalBox.ToSharedRef()
		];

	AssetT = AssetType::MeshType;
	RefreshContent();
}
void SMeshSelectionScrollBox::RefreshContent()
{
	HorizontalBox->ClearChildren();
	ScrollBox->ClearChildren();

	if (MeshDataAsset.IsValid()) {

		if (AssetT == AssetType::MeshType) {
			DisplayMeshes(MeshDataAsset->MeshDataArray);
			HorizontalBox->AddSlot()
				[
					ScrollBox.ToSharedRef()
				];
		}
		if (AssetT == AssetType::MaterialType) {
			DisplayMaterials(MeshDataAsset->MaterialDataArray);
			HorizontalBox->AddSlot()
				[
					ScrollBox.ToSharedRef()
				];
		}
		if (AssetT == AssetType::TextureType) {
			DisplayTextures(MeshDataAsset->TextureDataArray);
			HorizontalBox->AddSlot()
				[
					ScrollBox.ToSharedRef()
				];
		}

		
		
		
	}
}


void SMeshSelectionScrollBox::DisplayTextures(TArray<FTextureData> DataArray)
{
	for (const FTextureData& Data : DataArray) {
		if (Data.Type) {
			if (UTexture2D* ThumbnailTexture = Cast<UTexture2D>(Data.Type)) {
				// Create a brush for the thumbnail image
				FSlateBrush* ThumbnailBrush = new FSlateBrush();
				ThumbnailBrush->SetResourceObject(Data.Type);
				ThumbnailBrush->ImageSize = FVector2D(ThumbnailSize * 50, ThumbnailSize * 50);

				// Create the thumbnail image widget
				TSharedPtr<SImage> ThumbnailImage = SNew(SImage)
					.Image(ThumbnailBrush)
					.Cursor(EMouseCursor::Hand)
					.OnMouseButtonDown_Lambda([this, Data](const FGeometry& MouseGeometry, const FPointerEvent& MouseEvent) {
					if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
						OnTextureThumbnailSelected.ExecuteIfBound(Data);
						return FReply::Handled();
					}
					return FReply::Unhandled();
						});

				// Style for the text block
				const FTextBlockStyle TextStyle = FCoreStyle::Get().GetWidgetStyle<FTextBlockStyle>("NormalText");

				// Create the text block widget
				TSharedPtr<STextBlock> TextBlock = SNew(STextBlock)
					.Text(Data.Name)
					.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 16))
					.Justification(ETextJustify::Center)
					.ColorAndOpacity(TextStyle.ColorAndOpacity);

				// Add border around thumbnail image
				TSharedPtr<SBorder> ThumbnailBorder = SNew(SBorder)
					.BorderBackgroundColor(FLinearColor::Black) // Border color
					.BorderImage(FCoreStyle::Get().GetBrush("Button.Background")) // Border style
					.Padding(FMargin(1)) // Padding around the thumbnail
					[
						ThumbnailImage.ToSharedRef()
					];

				// Add border around text block
				TSharedPtr<SBorder> TextBlockBorder = SNew(SBorder)
					.BorderBackgroundColor(FLinearColor::Black) // Border color
					.BorderImage(FCoreStyle::Get().GetBrush("Button.Background")) // Border style
					.Padding(FMargin(5)) // Padding around the text
					[
						TextBlock.ToSharedRef()
					];

				// Add thumbnail image and text block to the scroll box
				ScrollBox->AddSlot()
					[
						SNew(SVerticalBox)
							+ SVerticalBox::Slot()
							.AutoHeight()
							[
								ThumbnailBorder.ToSharedRef()
							]
							+ SVerticalBox::Slot()
							.AutoHeight()
							[
								TextBlockBorder.ToSharedRef()
							]
					];
			}
		}
	}
	
}



void SMeshSelectionScrollBox::DisplayMaterials(TArray<FMaterialData> DataArray)
{
	for (const FMaterialData& Data : DataArray) {
		if (Data.Icon) {
			if (UTexture2D* ThumbnailTexture = Cast<UTexture2D>(Data.Icon)) {
				// Create a brush for the thumbnail image
				FSlateBrush* ThumbnailBrush = new FSlateBrush();
				ThumbnailBrush->SetResourceObject(Data.Icon);
				ThumbnailBrush->ImageSize = FVector2D(ThumbnailSize * 50, ThumbnailSize * 50);

				// Create the thumbnail image widget
				TSharedPtr<SImage> ThumbnailImage = SNew(SImage)
					.Image(ThumbnailBrush)
					.Cursor(EMouseCursor::Hand)
					.OnMouseButtonDown_Lambda([this, Data](const FGeometry& MouseGeometry, const FPointerEvent& MouseEvent) {
					if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
						OnMaterialThumbnailSelected.ExecuteIfBound(Data);
						return FReply::Handled();
					}
					return FReply::Unhandled();
						});

				// Style for the text block
				const FTextBlockStyle TextStyle = FCoreStyle::Get().GetWidgetStyle<FTextBlockStyle>("NormalText");

				// Create the text block widget
				TSharedPtr<STextBlock> TextBlock = SNew(STextBlock)
					.Text(Data.Name)
					.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 16))
					.Justification(ETextJustify::Center)
					.ColorAndOpacity(TextStyle.ColorAndOpacity);

				// Add border around thumbnail image
				TSharedPtr<SBorder> ThumbnailBorder = SNew(SBorder)
					.BorderBackgroundColor(FLinearColor::Black) // Border color
					.BorderImage(FCoreStyle::Get().GetBrush("Button.Background")) // Border style
					.Padding(FMargin(1)) // Padding around the thumbnail
					[
						ThumbnailImage.ToSharedRef()
					];

				// Add border around text block
				TSharedPtr<SBorder> TextBlockBorder = SNew(SBorder)
					.BorderBackgroundColor(FLinearColor::Black) // Border color
					.BorderImage(FCoreStyle::Get().GetBrush("Button.Background")) // Border style
					.Padding(FMargin(5)) // Padding around the text
					[
						TextBlock.ToSharedRef()
					];

				// Add thumbnail image and text block to the scroll box
				ScrollBox->AddSlot()
					[
						SNew(SVerticalBox)
							+ SVerticalBox::Slot()
							.AutoHeight()
							[
								ThumbnailBorder.ToSharedRef()
							]
							+ SVerticalBox::Slot()
							.AutoHeight()
							[
								TextBlockBorder.ToSharedRef()
							]
					];
			
			}
		}
	}
}
	



void SMeshSelectionScrollBox::DisplayMeshes(TArray<FMeshData> DataArray)
{
	for (const FMeshData& Data : DataArray) {
		if (Data.Icon) {
			if (UTexture2D* ThumbnailTexture = Cast<UTexture2D>(Data.Icon)) {
				// Create a brush for the thumbnail image
				FSlateBrush* ThumbnailBrush = new FSlateBrush();
				ThumbnailBrush->SetResourceObject(Data.Icon);
				ThumbnailBrush->ImageSize = FVector2D(ThumbnailSize * 50, ThumbnailSize * 50);

				// Create the thumbnail image widget
				TSharedPtr<SImage> ThumbnailImage = SNew(SImage)
					.Image(ThumbnailBrush)
					.Cursor(EMouseCursor::Hand)
					.OnMouseButtonDown_Lambda([this, Data](const FGeometry& MouseGeometry, const FPointerEvent& MouseEvent) {
					if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
						OnMeshThumbnailSelected.ExecuteIfBound(Data);
						return FReply::Handled();
					}
					return FReply::Unhandled();
						});

				// Style for the text block
				const FTextBlockStyle TextStyle = FCoreStyle::Get().GetWidgetStyle<FTextBlockStyle>("NormalText");

				// Create the text block widget
				TSharedPtr<STextBlock> TextBlock = SNew(STextBlock)
					.Text(Data.Name)
					.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 16))
					.Justification(ETextJustify::Center)
					.ColorAndOpacity(TextStyle.ColorAndOpacity);

				// Add border around thumbnail image
				TSharedPtr<SBorder> ThumbnailBorder = SNew(SBorder)
					.BorderBackgroundColor(FLinearColor::Black) // Border color
					.BorderImage(FCoreStyle::Get().GetBrush("Button.Background")) // Border style
					.Padding(FMargin(1)) // Padding around the thumbnail
					[
						ThumbnailImage.ToSharedRef()
					];

				// Add border around text block
				TSharedPtr<SBorder> TextBlockBorder = SNew(SBorder)
					.BorderBackgroundColor(FLinearColor::Black) // Border color
					.BorderImage(FCoreStyle::Get().GetBrush("Button.Background")) // Border style
					.Padding(FMargin(5)) // Padding around the text
					[
						TextBlock.ToSharedRef()
					];

				// Add thumbnail image and text block to the scroll box
				ScrollBox->AddSlot()
					[
						SNew(SVerticalBox)
							+ SVerticalBox::Slot()
							.AutoHeight()
							[
								ThumbnailBorder.ToSharedRef()
							]
							+ SVerticalBox::Slot()
							.AutoHeight()
							[
								TextBlockBorder.ToSharedRef()
							]
					];
			}
		}
	}
	
}



END_SLATE_FUNCTION_BUILD_OPTIMIZATION
