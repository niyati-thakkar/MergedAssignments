// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInterface.h"
#include "StaircaseActor.generated.h"

UENUM(BlueprintType)
enum class EStairType : uint8 {
	ClosedStairs,
	OpenStairs,
	BoxStairs,
};

USTRUCT(BlueprintType)
struct FSubStair
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StairComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* RightGlassRailing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* LeftGlassRailing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* RightGlassRailingTopWood;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* LeftGlassRailingTopWood;

};


UCLASS()
class ASSIGNMENT_1_2_API AStaircaseActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStaircaseActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NumberOfStairs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StairsLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StairsWidth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StairsDepth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* SourceMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stairs Properties")
	UMaterialInterface* StairsMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stairs Properties")
	UMaterialInterface* GlassMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stairs Properties")
	UStaticMesh* GlassMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stairs Properties")
	UMaterialInterface* HandleMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EStairType StairType;

protected:
	// Variable definitions (now protected)
	TArray<FSubStair*> OutMeshes;

	float StairSpacing;

	// Railing and handle dimensions
	float RailingWidth;
	float RailingThickness;
	float RailingDepth;
	float HandleWidth;
	float HandleLength;
	float RailingLengthOffset;
	float RailingWidthOffset;
	float RailingDepthOffset;
	float HandleDepth;
	float StepDepth;
	float HandleLengthOffset;  // Half width for left railing placement
	float HandleWidthOffset;  // Half width for right railing placement
	float HandleDepthOffset;
	virtual void OnConstruction(const FTransform& transform) override;
	void CreateStaircaseComponent(int Index);
	void DestoryStaircaseComponents(FSubStair* obj);
private:
	USceneComponent* SceneComponentStairs;
};



