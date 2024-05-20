// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveArchController.h"

void AInteractiveArchController::BeginPlay() {
	Super::BeginPlay();

	if (SelectionWidgetClass) {
		SelectionWidget = CreateWidget<USelectionWidget>(this, SelectionWidgetClass);
		if (SelectionWidget) {
			SelectionWidget->MeshThumbnailDelegate.AddDynamic(this, &AInteractiveArchController::SpawnMesh);
			SelectionWidget->MaterialThumbnailDelegate.AddDynamic(this, &AInteractiveArchController::ApplyMaterial);
			SelectionWidget->TextureThumbnailDelegate.AddDynamic(this, &AInteractiveArchController::ApplyTexture);
		}
	}

	if (DisplayWidgetClass)
	{
		WallWidget = CreateWidget<UDisplayWidget>(this, DisplayWidgetClass);
		WallWidget->AddToViewport();
		if(!bIsWallGenerator)
		{
			WallWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	DelegateLog.BindUFunction(this, FName("LogMessage"));
	DelegateLog.Execute(FString("hello"), FColor::White);
	UserWall = GetWorld()->SpawnActor<AWallGenerator>();

	Index = 0;
}



void AInteractiveArchController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent)) {
		UInputMappingContext* IMC = NewObject<UInputMappingContext>();

		UInputAction* ToggleControlAction = NewObject<UInputAction>();

		UInputAction* LeftClickAction = NewObject<UInputAction>();
		LeftClickAction->ValueType = EInputActionValueType::Axis3D;

		UInputAction* GenerateComponentAction = NewObject<UInputAction>();
		UInputAction* DestroyComponentAction = NewObject<UInputAction>();
		UInputAction* DestroyMeshAction = NewObject<UInputAction>();
		UInputAction* ClearLevelAction = NewObject<UInputAction>();

		UInputAction* HideUI = NewObject<UInputAction>();

		IMC->MapKey(ToggleControlAction, EKeys::I);

		IMC->MapKey(LeftClickAction, EKeys::LeftMouseButton);

		IMC->MapKey(GenerateComponentAction, EKeys::N);
		IMC->MapKey(DestroyComponentAction, EKeys::B);
		IMC->MapKey(DestroyMeshAction, EKeys::Z);
		IMC->MapKey(ClearLevelAction, EKeys::C);

		IMC->MapKey(HideUI, EKeys::Tab);

		EIC->BindAction(ToggleControlAction, ETriggerEvent::Completed, this, &AInteractiveArchController::ToggleController);

		EIC->BindAction(LeftClickAction, ETriggerEvent::Completed, this, &AInteractiveArchController::GetMouseClick);
		EIC->BindAction(GenerateComponentAction, ETriggerEvent::Completed, this, &AInteractiveArchController::GenerateNewWallComponent);
		EIC->BindAction(DestroyComponentAction, ETriggerEvent::Completed, this, &AInteractiveArchController::DestroyWallComponent);
		EIC->BindAction(DestroyMeshAction, ETriggerEvent::Completed, this, &AInteractiveArchController::DestroyWallMesh);
		EIC->BindAction(ClearLevelAction, ETriggerEvent::Completed, this, &AInteractiveArchController::DestroyAllWalls);

		EIC->BindAction(HideUI, ETriggerEvent::Completed, this, &AInteractiveArchController::HideUI);

		const auto LocalPlayer = GetLocalPlayer();

		if (LocalPlayer)
		{
			if (UEnhancedInputLocalPlayerSubsystem* SubSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				// Add the input mapping context to the local player subsystem
				SubSystem->AddMappingContext(IMC, 0);
			}
		}
		bShowMouseCursor = true;
		DefaultMouseCursor = EMouseCursor::Crosshairs;
	}
}

void AInteractiveArchController::ToggleController(const FInputActionValue& InputAction)
{

	bIsWallGenerator = !bIsWallGenerator;

	if(bIsWallGenerator)
	{
		SelectionWidget->SetVisibility(ESlateVisibility::Hidden);
		WallWidget->SetVisibility(ESlateVisibility::Visible);
	}else
	{
		SelectionWidget->SetVisibility(ESlateVisibility::Visible);
		WallWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}


void AInteractiveArchController::SpawnMesh(const FMeshData& MeshData)
{
	if (ArchMeshActor) {
		ArchMeshActor->GetStaticMeshComponent()->SetStaticMesh(MeshData.Type);
		return;
	}
	if (MeshData.Type) {
		FBox BoundingBox = MeshData.Type->GetBoundingBox();
		FVector MinBounds = BoundingBox.Min;
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ArchMeshActor = GetWorld()->SpawnActor<AArchMeshActor>(LastHitLocation - MinBounds, FRotator(0), SpawnParameters);
		if (ArchMeshActor) {
			ArchMeshActor->SetMobility(EComponentMobility::Movable);
			ArchMeshActor->GetStaticMeshComponent()->SetStaticMesh(MeshData.Type);

		}
	}
	SelectionWidget->MeshScrollBox->SetVisibility(ESlateVisibility::Visible);
	SelectionWidget->MaterialScrollBox->SetVisibility(ESlateVisibility::Visible);
	SelectionWidget->TextureScrollBox->SetVisibility(ESlateVisibility::Visible);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("hello"));

}

void AInteractiveArchController::ApplyMaterial(const FMaterialData& MaterialData)
{

	if (MaterialData.Type != nullptr) {
		if (ArchMeshActor) {
			ArchMeshActor->SetMobility(EComponentMobility::Movable);
			ArchMeshActor->GetStaticMeshComponent()->SetMaterial(0, MaterialData.Type);
		}
	}

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("material applied"));

}


void AInteractiveArchController::ApplyTexture(const FTextureData& TextureData)
{
	if (TextureData.Type && ArchMeshActor)
	{
		UStaticMeshComponent* MeshComponent = ArchMeshActor->GetStaticMeshComponent();
		if (MeshComponent)
		{
			TArray<UMaterialInterface*> Materials = MeshComponent->GetMaterials();
			int i = 0;
			for (UMaterialInterface* Material : Materials)
			{
				if (Material)
				{
					UMaterialInstanceDynamic* DynMaterial = Cast<UMaterialInstanceDynamic>(Material);
					if (!DynMaterial)
					{
						DynMaterial = UMaterialInstanceDynamic::Create(Material, NULL);
						MeshComponent->SetMaterial(i++, DynMaterial);
					}
					// Convert FText to FString and then to FName
					DynMaterial->SetTextureParameterValue(FName("TextureParam"), TextureData.Type);
					// Replace "TextureParameterName" with the name of the texture parameter in your material
				}
			}
		}
	}
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("texture applied"));
}


void AInteractiveArchController::GetMouseClick(const FInputActionValue& InputAction) {
	float MouseX, MouseY;
	FVector WorldLocation, WorldDirection;
	if (GetMousePosition(MouseX, MouseY)) {
		if (DeprojectMousePositionToWorld(WorldLocation, WorldDirection)) {
			FHitResult HitResult;
			FVector TraceEnd = WorldLocation + (WorldDirection * 10000.f);

			FCollisionQueryParams QueryParams;
			QueryParams.bTraceComplex = true;
			QueryParams.AddIgnoredActor(GetPawn());

			if (GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, TraceEnd, ECC_Visibility, QueryParams)) {
				if (HitResult.GetActor()) {

					if(bIsWallGenerator)
					{
						
						if (HitResult.IsValidBlockingHit())
						{
							// Print the location to the screen
							FVector Location = HitResult.Location;
							GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Mouse Click Location: X=%f, Y=%f, Z=%f"), Location.X, Location.Y, Location.Z));

							UserWall->GenerateSplineMesh(Location);
							DelegateLog.Execute(FString("Got Location"), FColor::Yellow);
						}
						else
						{
							DelegateLog.Execute(FString("No valid location found under the mouse cursor"), FColor::Yellow);
						}
					}else
					{
						LastHitLocation = HitResult.Location;
						if (SelectionWidget && !SelectionWidget->IsInViewport()) {
							SelectionWidget->AddToViewport();
							SelectionWidget->InitializeWidget(MeshDataAsset);
						}
						ArchMeshActor = Cast<AArchMeshActor>(HitResult.GetActor());
						if (ArchMeshActor) {
							SelectionWidget->MeshScrollBox->SetVisibility(ESlateVisibility::Visible);
							SelectionWidget->MaterialScrollBox->SetVisibility(ESlateVisibility::Visible);
							SelectionWidget->TextureScrollBox->SetVisibility(ESlateVisibility::Visible);
						}
						else {
							SelectionWidget->MeshScrollBox->SetVisibility(ESlateVisibility::Visible);
							SelectionWidget->MaterialScrollBox->SetVisibility(ESlateVisibility::Hidden);
							SelectionWidget->TextureScrollBox->SetVisibility(ESlateVisibility::Hidden);
						}
					}
					
				}
			}
		}
	}
}



void AInteractiveArchController::HideUI()
{
	if(!bIsWallGenerator)
	{
		SelectionWidget->MeshScrollBox->SetVisibility(ESlateVisibility::Hidden);
		SelectionWidget->MaterialScrollBox->SetVisibility(ESlateVisibility::Hidden);
		SelectionWidget->TextureScrollBox->SetVisibility(ESlateVisibility::Hidden);
	}
	
}

void AInteractiveArchController::DestroyWallMesh()
{
	if(bIsWallGenerator)
	{
		if (UserWall->DestroySplineMesh()) {
			DelegateLog.Execute(FString("Wall Mesh Deleted Successfully"), FColor::Green);
		}
		else {
			DelegateLog.Execute(FString("No More Walls to Destroy"), FColor::Orange);
		}
	}
	
}

void AInteractiveArchController::DestroyWallComponent()
{
	if (bIsWallGenerator)
	{
		if (UserWall->DestroyAllSplineMeshes()) {
			DelegateLog.Execute(FString("Wall Component Deleted Successfully"), FColor::Green);
		}
		else {
			DelegateLog.Execute(FString("No More Walls to Destroy"), FColor::Orange);
		}
	}
}

void AInteractiveArchController::DestroyAllWalls()
{
	if (bIsWallGenerator)
	{
		if (UserWall->DestroyAllSplineComponents()) {
			DelegateLog.Execute(FString("Cleared Successfully"), FColor::Purple);
		}
	}
}

void AInteractiveArchController::GenerateNewWallComponent()
{
	if (bIsWallGenerator)
	{
		if (UserWall->GenerateNewSplineComponent()) {
			DelegateLog.Execute(FString("New Wall Added"), FColor::Green);
		}
	}
}

void AInteractiveArchController::LogMessage(const FString& Message, const FLinearColor& ColorOpacity)
{

	WallWidget->String_Message = Message;
	WallWidget->Color_Opacity = ColorOpacity;
}

void AInteractiveArchController::SpawnPawn() {
	if (PawnTypeDataTable) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("got Data table"));

		FPawnDataTable* CurrentPawnType = TypesOfPawns[Index];
		if (CurrentPawnType) {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("got struct"));
			if (UWorld* CurrentWorld = GetWorld()) {
				/*if(CurrentPawnType->PawnType == EPawnType::FirstPerson)
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("1"));
				if (CurrentPawnType->PawnType == EPawnType::ThirdPerson)
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("2"));
				if (CurrentPawnType->PawnType == EPawnType::TopDown)
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("3"));*/
				if (CurrentPawn) {
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, TEXT("P Key Pressed! Spawning Another Pawn..."));
					CurrentPawnLocation = CurrentPawn->GetActorLocation();
					CurrentPawnRotation = CurrentPawn->GetActorRotation();
					CurrentPawn->Destroy();
					CurrentPawn = nullptr;
				}
				FActorSpawnParameters SpawnParameter;
				SpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
				APawn* tempPawn = CurrentWorld->SpawnActor<APawn>(CurrentPawnType->PawnClass, CurrentPawnLocation, CurrentPawnRotation, SpawnParameter);
				SetEnhancedInputToggle();
				if (tempPawn != nullptr) {
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, TEXT("Spawned Pawn"));

					Possess(tempPawn);

					if (tempPawn->GetController())
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, TEXT("Possessed Pawn"));
					CurrentPawn = tempPawn;
					if (CurrentPawnType->PawnType == EPawnType::TopDown) {
						bShowMouseCursor = true;
						/*bEnableClickEvents = true;
						bEnableTouchEvents = true;
						bEnableMouseOverEvents = true;
						DefaultMouseCursor = EMouseCursor::Default;
						DefaultClickTraceChannel = ECollisionChannel::ECC_Visibility;*/
						//DisplayAttributes(Cast<ABaseCharacter>(SpawnedPawn)->PawnAttributeAsset);
					}
					else {
						bShowMouseCursor = false;
						/*bEnableClickEvents = false;
						bEnableTouchEvents = false;
						bEnableMouseOverEvents = false;*/
						//DisplayAttributes(Cast<ABaseCharacter>(SpawnedPawn)->PawnAttributeAsset);
					}
					
				}
			}

		}
	}
	Toggle();


}

void AInteractiveArchController::Toggle() {
	Index = ((Index + 1) % Size);
}
void AInteractiveArchController::BeginPlay()
{
	//SpawnPawn();
}