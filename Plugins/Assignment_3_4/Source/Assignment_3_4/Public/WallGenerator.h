// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/SplineComponent.h>
#include "Components/SceneComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstance.h"
#include "WallGenerator.generated.h"

UCLASS()
class ASSIGNMENT_3_4_API AWallGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWallGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	USplineComponent* SplineComponent;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Default)
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Default)
	UStaticMesh* SourceMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Default)
	UMaterialInterface* WallMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Default)
	TEnumAsByte<ESplineMeshAxis::Type> SplineMeshAxis;

	UFUNCTION(BlueprintCallable)
	bool GenerateSplineMesh(FVector SplinePoint);
	
	UFUNCTION(BlueprintCallable)
	bool DestroySplineMesh();

	UFUNCTION(BlueprintCallable)
	bool DestroyAllSplineMeshes();

	UFUNCTION(BlueprintCallable)
	bool GenerateNewSplineComponent();
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Default)
	UFUNCTION(BlueprintCallable)
	bool DestroyAllSplineComponents();

	UPROPERTY()
	TArray<USplineComponent*> SplineComponents;

	UPROPERTY()
	TArray<USplineMeshComponent*> SplineMeshes;

};
