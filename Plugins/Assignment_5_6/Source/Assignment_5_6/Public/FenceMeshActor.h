// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VerticalRailActor.h"
#include <Components/SplineComponent.h>
#include "Components/SceneComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstance.h"
#include "GameFramework/Actor.h"
#include "FenceDataAsset.h"
#include "FenceMeshActor.generated.h"




UCLASS()
class ASSIGNMENT_5_6_API AFenceMeshActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFenceMeshActor();

	void GenerateVerticalMesh();
	void OnConstruction(const FTransform& Transform) override;
	void GenerateHorizontalMesh();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY()
	TSubclassOf < AVerticalRailActor > VerticalRailActorClass;

	UPROPERTY()
	USceneComponent* SceneComponent;

	UPROPERTY()
	USplineComponent* FenceSplineComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	TEnumAsByte<ESplineMeshAxis::Type> SplineMeshAxis;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EFenceType FenceType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	EMaterialType MaterialType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	UStaticMesh* HorizontalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	FVector HorizontalMeshOffset{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	int TileY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fence Data")
	UFenceDataAsset* FenceData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	FFenceProperties FenceProperty;
	
	UPROPERTY()
	TArray<AActor*> SplineComponents;

	UPROPERTY()
	TArray<UStaticMeshComponent*> SplineMeshes;

	UPROPERTY()
	UStaticMesh* SourceMesh;

	UPROPERTY()
	UMaterialInterface* SourceMaterial;

	UFUNCTION(BlueprintCallable)
	void ClearExistingMeshes();

	UFUNCTION(BlueprintCallable)
	void ReplacePillarsWithRails();

	UFUNCTION(BlueprintCallable)
	void ReplaceHorizontalMeshsWithProceduralMesh();
};