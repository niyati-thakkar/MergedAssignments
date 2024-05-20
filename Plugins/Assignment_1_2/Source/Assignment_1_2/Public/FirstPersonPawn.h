// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include <Components/CapsuleComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include "EnhancedActionKeyMapping.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include <GameFramework/FloatingPawnMovement.h>
#include "PawnAttributeAsset.h"
#include "FirstPersonPawn.generated.h"



UCLASS()
class ASSIGNMENT_1_2_API AFirstPersonPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AFirstPersonPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


public:
	UPROPERTY(VisibleDefaultsOnly, Category=Defaults)
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleDefaultsOnly, Category=Defaults)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleDefaultsOnly, Category=Defaults)
	UCameraComponent* Camera;

	UPROPERTY(VisibleDefaultsOnly, Category=Defaults)
	UFloatingPawnMovement* FloatingPawnMovement;

	void Look(const FInputActionValue& LookAction);

	void Move(const FInputActionValue& LookAction);
	//UPROPERTY(VisibleDefaultsOnly, Category=Defaults)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Asset")
	UPawnAttributeAsset* PawnAttributes;
};
