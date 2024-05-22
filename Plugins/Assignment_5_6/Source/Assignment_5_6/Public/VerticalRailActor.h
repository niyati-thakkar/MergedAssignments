// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "VerticalRailActor.generated.h"


UCLASS()
class ASSIGNMENT_5_6_API AVerticalRailActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVerticalRailActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
    // Called in the editor when properties are changed
    virtual void OnConstruction(const FTransform& Transform) override;

    // Procedural Mesh Component
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UProceduralMeshComponent* ProcMeshComponent;

    // Properties for customization
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rail Settings")
    float TopMeshLerpValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rail Settings")
    int32 MeshTypeIndex;

    // Functions to generate different types of top meshes
    void GenerateBottomMesh();
    void GenerateTopMesh(int32 MeshTypeIndex);
    void CombineMeshes();
    void GetBottomMeshData(TArray<FVector>& Vertices, TArray<int32>& Triangles);
    void CreateSphere(float Radius, int32 Segments, TArray<FVector>& Vertices, TArray<int32>& Triangles);
    void CreateCone(float Height, float Radius, int32 Sides, TArray<FVector>& Vertices, TArray<int32>& Triangles);
    void CreatePyramid(float Height, float BaseRadius, TArray<FVector>& Vertices, TArray<int32>& Triangles);
    // Helper function to create a box
    void CreateBox(float Width, float Depth, float Height, TArray<FVector>& Vertices, TArray<int32>& Triangles);
};