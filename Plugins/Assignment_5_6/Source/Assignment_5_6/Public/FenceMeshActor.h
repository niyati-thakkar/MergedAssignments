// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VerticalRailActor.h"
#include <Components/SplineComponent.h>
#include "Components/SceneComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Engine/DataTable.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstance.h"
#include "GameFramework/Actor.h"
#include "FenceMeshActor.generated.h"


USTRUCT()
struct FFenceProperties
{
	GENERATED_BODY()

	UPROPERTY()
	float length;

	UPROPERTY()
	float width;

	UPROPERTY()
	float height;

	UPROPERTY()
	float spacing;
};

USTRUCT(BlueprintType)
struct FFenceTypes : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY()
	FString Name;

	UPROPERTY()
	AVerticalRailActor* Fence;

};
UCLASS()
class ASSIGNMENT_5_6_API AFenceMeshActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFenceMeshActor();
	void ClearExistingPillars();

	void OnConstruction(const FTransform& Transform) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void ReplacePillarsWithRails();
	void ClearExistingBambooSticks();

	//UPROPERTY()
	//USplineMeshComponent* FenceSplineComponent;

	UPROPERTY()
	TSubclassOf < AVerticalRailActor > VerticalRailActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	USplineComponent* FenceSplineComponent;

	TArray<AVerticalRailActor> RailingObjects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	UStaticMesh* SourceMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	UMaterialInterface* FenceMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	TEnumAsByte<ESplineMeshAxis::Type> SplineMeshAxis;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	int32 PillarSpacing;

	UPROPERTY()
	TArray<USplineComponent*> SplineComponents;

	UPROPERTY()
	TArray<UStaticMeshComponent*> SplineMeshes;

	UPROPERTY()
	UStaticMesh* BambooStickMesh;
	FVector BambooStickOffset{};
};
