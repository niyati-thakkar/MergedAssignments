// Fill out your copyright notice in the Description page of Project Settings.


#include "A3PlayerController.h"

AA3PlayerController::AA3PlayerController()
{
	
	
}


void AA3PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent)) {
		UInputMappingContext* InputMappingContext = NewObject<UInputMappingContext>();

		// Look Action
		UInputAction* GenerateMeshAction = NewObject<UInputAction>();
		UInputAction* GenerateComponentAction = NewObject<UInputAction>();
		UInputAction* DestroyComponentAction = NewObject<UInputAction>();
		UInputAction* DestroyMeshAction = NewObject<UInputAction>();
		UInputAction* ClearLevelAction = NewObject<UInputAction>();
		GenerateMeshAction->ValueType = EInputActionValueType::Axis3D;



		InputMappingContext->MapKey(GenerateMeshAction, EKeys::LeftMouseButton);
		InputMappingContext->MapKey(GenerateComponentAction, EKeys::N);
		InputMappingContext->MapKey(DestroyComponentAction, EKeys::B);
		InputMappingContext->MapKey(DestroyMeshAction, EKeys::Z);
		InputMappingContext->MapKey(ClearLevelAction, EKeys::C);

		EnhancedInputComponent->BindAction(GenerateMeshAction, ETriggerEvent::Completed, this, &AA3PlayerController::GetLocation);
		EnhancedInputComponent->BindAction(GenerateComponentAction, ETriggerEvent::Completed, this, &AA3PlayerController::GenerateNewWallComponent);
		EnhancedInputComponent->BindAction(DestroyComponentAction, ETriggerEvent::Completed, this, &AA3PlayerController::DestroyWallComponent);
		EnhancedInputComponent->BindAction(DestroyMeshAction, ETriggerEvent::Completed, this, &AA3PlayerController::DestroyWallMesh);
		EnhancedInputComponent->BindAction(ClearLevelAction, ETriggerEvent::Completed, this, &AA3PlayerController::DestroyAllWalls);

		const auto LocalPlayer = GetLocalPlayer();

		if (LocalPlayer)
		{
			if (UEnhancedInputLocalPlayerSubsystem* SubSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				// Add the input mapping context to the local player subsystem
				SubSystem->AddMappingContext(InputMappingContext, 0);
			}
		}
		bShowMouseCursor = true;
		
	}
	
	
	
}
void AA3PlayerController::GetLocation() {
	FHitResult HitResult;
	GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

	// Check if the hit was successful
	if (HitResult.IsValidBlockingHit())
	{
		// Print the location to the screen
		FVector Location = HitResult.Location;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Mouse Click Location: X=%f, Y=%f, Z=%f"), Location.X, Location.Y, Location.Z));

		UserWall->GenerateSplineMesh(Location);
	}
	else
	{
		DelegateLog.Execute(FString("No valid location found under the mouse cursor"), FColor::Yellow);
	}
	
}

void AA3PlayerController::BeginPlay()
{
	Super::BeginPlay();
	DelegateLog.BindUFunction(this, FName("LogMessage"));
	//DelegateLog.Execute(FString(""), FColor::White);
	UserWall = GetWorld()->SpawnActor<AWallGenerator>();
}

void AA3PlayerController::DestroyWallMesh()
{
	if (UserWall->DestroySplineMesh()) {
		DelegateLog.Execute(FString("Wall Mesh Deleted Successfully"), FColor::Blue);
	}
	else {
		DelegateLog.Execute(FString("No More Walls to Destroy"), FColor::Orange);
	}
}

void AA3PlayerController::DestroyWallComponent()
{
	if (UserWall->DestroyAllSplineMeshes()) {
		DelegateLog.Execute(FString("Wall Component Deleted Successfully"), FColor::Blue);
	}
	else {
		DelegateLog.Execute(FString("No More Walls to Destroy"), FColor::Orange);
	}
}

void AA3PlayerController::DestroyAllWalls()
{
	if (UserWall->DestroyAllSplineComponents()) {
		DelegateLog.Execute(FString("Cleared Successfully"), FColor::Purple);
	}
}

void AA3PlayerController::GenerateNewWallComponent()
{
	if (UserWall->GenerateNewSplineComponent()) {
		DelegateLog.Execute(FString("New Wall Added"), FColor::Green);
	}
}

