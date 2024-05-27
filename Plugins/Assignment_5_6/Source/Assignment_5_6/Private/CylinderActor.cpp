// Fill out your copyright notice in the Description page of Project Settings.


#include "CylinderActor.h"

// Sets default values
ACylinderActor::ACylinderActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    ProcMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProcMeshComponent"));
    RootComponent = ProcMeshComponent;

}

// Called when the game starts or when spawned
void ACylinderActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACylinderActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACylinderActor::OnConstruction(const FTransform& Transform)
{
   
}
// CylinderGenerator.cpp




void ACylinderActor::GenerateCylinder(float Radius, float Height, int32 Segments)
{
    // ProcMeshComponent->ClearAllMeshSections();

    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UVs;
    TArray<FProcMeshTangent> Tangents;
    TArray<FLinearColor> Colors;

    // Generate the vertices, normals, UVs, and tangents for the cylinder
    for (int32 s = 0; s <= Segments; s++)
    {
        float Angle = static_cast<float>(s) / Segments * 2.0f * PI;
        float Y = Radius * FMath::Cos(Angle); // Rotate along Y-axis for horizontal cylinder
        float Z = Radius * FMath::Sin(Angle);

        Vertices.Add(FVector(Height / 2.0f, Y, Z));  // Adjust the position along X-axis for horizontal cylinder
        Vertices.Add(FVector(-Height / 2.0f, Y, Z)); // Adjust the position along X-axis for horizontal cylinder

        Normals.Add(FVector(0.0f, FMath::Cos(Angle), FMath::Sin(Angle))); // Normals aligned with cylinder direction
        Normals.Add(FVector(0.0f, FMath::Cos(Angle), FMath::Sin(Angle)));

        UVs.Add(FVector2D(static_cast<float>(s) / Segments, 0.0f));
        UVs.Add(FVector2D(static_cast<float>(s) / Segments, 1.0f));

        FVector TangentX = FVector(0.0f, -FMath::Sin(Angle), FMath::Cos(Angle)); // Tangent aligned with cylinder direction
        FVector TangentY = FVector(1.0f, 0.0f, 0.0f); // Tangent along X-axis for horizontal cylinder
        FVector TangentZ = FVector(0.0f, 0.0f, 1.0f);

        Tangents.Add(FProcMeshTangent(TangentX, false));
        Tangents.Add(FProcMeshTangent(TangentY, false));
    }

    // Generate the triangle indices for the sides of the cylinder
    for (int32 s = 0; s < Segments; s++)
    {
        int32 i0 = s * 2;
        int32 i1 = i0 + 1;
        int32 i2 = (s + 1) * 2;
        int32 i3 = i2 + 1;

        // Outer side
        Triangles.Add(i0);
        Triangles.Add(i2);
        Triangles.Add(i1);

        Triangles.Add(i1);
        Triangles.Add(i2);
        Triangles.Add(i3);

        // Inner side (reverse winding)
        Triangles.Add(i0);
        Triangles.Add(i1);
        Triangles.Add(i2);

        Triangles.Add(i1);
        Triangles.Add(i3);
        Triangles.Add(i2);
    }

    // Add the cylinder caps
    AddCylinderCaps(Vertices, Triangles, Normals, UVs, Colors, Tangents, Radius, Height / 2.0f, Segments);
    AddCylinderCaps(Vertices, Triangles, Normals, UVs, Colors, Tangents, Radius, -Height / 2.0f, Segments);

    // Generate the vertex colors
    for (int32 i = 0; i < Vertices.Num(); i++)
    {
        Colors.Add(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
    }

    // Add the mesh to the procedural mesh component
    ProcMeshComponent->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, Colors, Tangents, true);
}



void ACylinderActor::AddCylinderCaps(
    TArray<FVector>& Vertices,
    TArray<int32>& Triangles,
    TArray<FVector>& Normals,
    TArray<FVector2D>& UVs,
    TArray<FLinearColor>& Colors,
    TArray<FProcMeshTangent>& Tangents,
    float Radius,
    float HalfHeight,
    int32 Segments)
{
    int32 CapCenterBottomIndex = Vertices.Num();
    Vertices.Add(FVector(-HalfHeight, 0.0f, 0.0f));
    Normals.Add(FVector(0.0f, -1.0f, 0.0f)); // Normal pointing downward
    UVs.Add(FVector2D(0.5f, 0.5f)); // Center UV
    Colors.Add(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
    Tangents.Add(FProcMeshTangent(FVector(1.0f, 0.0f, 0.0f), false));

    int32 CapCenterTopIndex = Vertices.Num();
    Vertices.Add(FVector(HalfHeight, 0.0f, 0.0f));
    Normals.Add(FVector(0.0f, 1.0f, 0.0f)); // Normal pointing upward
    UVs.Add(FVector2D(0.5f, 0.5f)); // Center UV
    Colors.Add(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
    Tangents.Add(FProcMeshTangent(FVector(1.0f, 0.0f, 0.0f), false));

    for (int32 s = 0; s <= Segments; s++)
    {
        
        float Angle = static_cast<float>(s) / Segments * 2.0f * PI;
        float X = Radius * FMath::Cos(Angle);
        float Y = Radius * FMath::Sin(Angle);

        FVector BottomVertex = FVector(-HalfHeight, X, Y);
        FVector TopVertex = FVector(HalfHeight, X, Y);

        Vertices.Add(BottomVertex);
        Vertices.Add(TopVertex);

        // Normals for the caps pointing outward
        FVector BottomNormal = FVector(0.0f, 0.0f, -1.0f); // Normal pointing downward
        FVector TopNormal = FVector(0.0f, 0.0f, 1.0f); // Normal pointing upward

        Normals.Add(BottomNormal);
        Normals.Add(TopNormal);

        // UVs mapping the vertices to a unit circle
        FVector2D BottomUV = FVector2D(0.5f + X / (2.0f * Radius), 0.5f + Y / (2.0f * Radius));
        FVector2D TopUV = FVector2D(0.5f + X / (2.0f * Radius), 0.5f + Y / (2.0f * Radius));

        UVs.Add(BottomUV);
        UVs.Add(TopUV);

        Colors.Add(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
        Colors.Add(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));

        Tangents.Add(FProcMeshTangent(FVector(1.0f, 0.0f, 0.0f), false));
        Tangents.Add(FProcMeshTangent(FVector(1.0f, 0.0f, 0.0f), false));

        if (s < Segments)
        {
            int32 BottomIndex0 = CapCenterBottomIndex + s * 2 + 2;
            int32 BottomIndex1 = BottomIndex0 + 2;

            int32 TopIndex0 = CapCenterTopIndex + s * 2 + 2;
            int32 TopIndex1 = TopIndex0 + 2;

            // Adding triangles for the caps
            Triangles.Add(CapCenterBottomIndex);
            Triangles.Add(BottomIndex0);
            Triangles.Add(BottomIndex1);

            Triangles.Add(CapCenterTopIndex);
            Triangles.Add(TopIndex1);
            Triangles.Add(TopIndex0);
        }
    }
}
