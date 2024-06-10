// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "GameFramework/Actor.h"
#include "SelectionArea.generated.h"

UCLASS()
class ASSIGNMENT_5_6_API ASelectionArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASelectionArea();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* ProcMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Dimensions;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius;

	UPROPERTY()
	UMaterialInterface* TranslucentMaterial;
	UPROPERTY()
	UMaterialInterface* DefaultMaterial;
	UPROPERTY()
	FVector LastMouseLocation;
	UPROPERTY()
	bool bDragging;
	UPROPERTY()
	bool bIsSphere;
	UPROPERTY()
	bool bIsSpawned;
	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void GenerateCube();
	UFUNCTION(BlueprintCallable)
	void GenerateSphere();
	UFUNCTION(BlueprintCallable)
	void UpdatePosition();
	UFUNCTION(BlueprintCallable)
	void SpawnGeneratedMesh();
	void BuildQuad(TArray<FVector>& InVertices, TArray<int32>& InTriangles, TArray<FVector>& InNormals, TArray<FProcMeshTangent>& InTangents, TArray<FVector2D>& InTexCoords, const FVector BottomLeft, const FVector BottomRight, const FVector TopRight, const FVector TopLeft, int32& VertexOffset, int32& TriangleOffset, const FVector Normal, const FProcMeshTangent Tangent);
};
