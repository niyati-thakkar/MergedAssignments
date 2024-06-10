// Fill out your copyright notice in the Description page of Project Settings.


#include "A5PlayerController.h"



void AA5PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	bShowMouseCursor = true;

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent)) {
		UInputMappingContext* IMC = NewObject<UInputMappingContext>();

		UInputAction* RightClickAction = NewObject<UInputAction>();
		RightClickAction->ValueType = EInputActionValueType::Axis2D;

		IMC->MapKey(RightClickAction, EKeys::RightMouseButton);

		EIC->BindAction(RightClickAction, ETriggerEvent::Completed, this, &AA5PlayerController::GetMouseClick);

		const auto LocalPlayer = GetLocalPlayer();

		if (LocalPlayer)
		{
			if (UEnhancedInputLocalPlayerSubsystem* SubSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				//SubSystem->ClearAllMappings();

				// Add the input mapping context to the local player subsystem
				SubSystem->AddMappingContext(IMC, 0);
			}
		}
		bShowMouseCursor = true;
		DefaultMouseCursor = EMouseCursor::Crosshairs;
	}
	SelectionArea = GetWorld()->SpawnActor<ASelectionArea>(FVector::ZeroVector, FRotator(0));
	MeshGenerator = GetWorld()->SpawnActor<AMeshGenerator>(FVector::ZeroVector, FRotator(0));
	isGenerating = false;
}

void AA5PlayerController::GetMouseClick(const FInputActionValue& InputAction) {

	if(!isGenerating)
	{
		SelectionArea->bIsSpawned = !SelectionArea->bIsSpawned;
		if (SelectionArea->bIsSpawned) {
			SelectionArea->SpawnGeneratedMesh();
			MeshGenerator->ScatterObjects();
		}
	}
	
				
}
void AA5PlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (WidgetBPClass)
	{
		W_MeshGenerationUI = CreateWidget<UUserWidget>(this, WidgetBPClass);
		W_MeshGenerationUI->AddToViewport();
		W_MeshGenerationUI->AddToViewport(0);
	}
	if(MeshGenerator)
	{
		MeshGenerator->ScatterProgressUpdated.AddDynamic(this, &AA5PlayerController::UpdateProgressBar);
	}
}