// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FirstPersonPawn.h"
#include "Engine/DataTable.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
//#include "PawnAttributeAsset.h"
#include "InputAction.h"
#include "GenericStructs.h"
#include "PawnAttributeAsset.h"
#include "CustomPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENT_1_2_API ACustomPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ACustomPlayerController();
	~ACustomPlayerController() = default;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	class UDataTable* PawnTypeDataTable{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	int32 Index{};
	int32 Size{};
	TArray<FPawnDataTable*> TypesOfPawns;
	void BeginPlay();
	void SpawnPawn();
	void Toggle();
	APawn* CurrentPawn;
	FVector CurrentPawnLocation;
	FRotator CurrentPawnRotation;
	void SetupInputComponent() override;
	void SetEnhancedInputToggle();


	UFUNCTION(BlueprintImplementableEvent)
	void DisplayAttributes(UPawnAttributeAsset* PawnAttributes);


};
