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
	void InitialPillarGeneration();

	// Called in the editor when properties are changed
    virtual void OnConstruction(const FTransform& Transform) override;
	void RoundTurnedCapital();

	// Procedural Mesh Component
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UProceduralMeshComponent* ProcMeshComponent;

    // Properties for customization
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rail Settings")
    float TopMeshLerpValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rail Settings")
    int32 MeshTypeIndex;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BottomHeight = 0.0f;

    //EditAnywhere, BlueprintReadWrite
    UPROPERTY()
    FVector Location = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Scale = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Index = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BottomSide = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TopSide = 0.0f;


    //UPROPERTY(EditAnywhere, BlueprintReadWrite)
    //UMaterialInstance* MaterialInstance;

	UFUNCTION(BlueprintCallable, Category = Defauls)
    void WindsorTurnedCapital();
	void PyramidTop();
	void ACornCapital();
	void GothicStarCapital();
	void RoundedOverTopCapital();


	UFUNCTION(BlueprintCallable, Category = Defauls)
    void GenerateCube(FVector Dimensions);

    UFUNCTION(BlueprintCallable, Category = Defauls)
    void GenerateSphere(float Radius, int32 Segments, int32 Rings);

    UFUNCTION(BlueprintCallable, Category = Defauls)
    void GenerateCylinder(float Radius, float Height, int32 Segments);

    UFUNCTION(BlueprintCallable, Category = Defauls)
    void GenerateCone(float Radius, float Height, int32 Segments);
   
    void GenerateBellShape(float BaseRadius, float Height, float RimRadius, float CurvatureFactor, int32 NumSlices, int
                           NumStacks);
    void GenerateInvertedSemiEggShape(int32 NumSegments, float RadiusX, float RadiusY, float Height, TArray<FVector>& Vertices, TArray<int32>& Triangles, TArray
                                      <FVector>& Normals, TArray<FVector2D>& UVs, TArray<FProcMeshTangent>& Tangents, TArray<FLinearColor>& VertexColors);
	void GenerateTorus(float InnerRadius, float OuterRadius, int32 RadialSegments, int32 TubularSegments);
	void GenerateCornShape(int32 NumSegments, float BaseRadiusX, float BaseRadiusY, float Height);
	void GeneratePyramid(float BaseLength, float Height);
	void GenerateFenceTop(float Radius, float Length, float Width);
	
	void GenerateSemiEggShape(int32 NumSegments, float RadiusX, float RadiusY, float Height, TArray<FVector>& Vertices, TArray<int32>& Triangles, TArray
	                          <FVector>& Normals, TArray<FVector2D>& UVs, TArray<FProcMeshTangent>& Tangents, TArray<FLinearColor>& VertexColors);
	int32 Segment = 0;
    void BuildQuad(TArray<FVector>& InVertices, TArray<int32>& InTriangles, TArray<FVector>& InNormals, TArray<FProcMeshTangent>& InTangents, TArray<FVector2D>& InTexCoords, const FVector BottomLeft, const FVector BottomRight, const FVector TopRight, const FVector TopLeft, int32& VertexOffset, int32& TriangleOffset, const FVector Normal, const FProcMeshTangent Tangent);

};