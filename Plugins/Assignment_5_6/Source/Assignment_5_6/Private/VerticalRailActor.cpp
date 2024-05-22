// Fill out your copyright notice in the Description page of Project Settings.


#include "VerticalRailActor.h"

// Sets default values
AVerticalRailActor::AVerticalRailActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProcMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProcMeshComponent"));
	RootComponent = ProcMeshComponent;

	TopMeshLerpValue = 0.0f;
	MeshTypeIndex = 0;
}

// Called when the game starts or when spawned
void AVerticalRailActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVerticalRailActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AVerticalRailActor::GenerateTopMesh(int32 MTI)
{
}

void AVerticalRailActor::CombineMeshes()
{
}


void AVerticalRailActor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    float SphereRadius = 10.0f;
    int32 SphereSegments = 16;

    // Arrays to hold the sphere's vertices and triangles
    TArray<FVector> SphereVertices;
    TArray<int32> SphereTriangles;

    // Generate the sphere
    CreateSphere(SphereRadius, SphereSegments, SphereVertices, SphereTriangles);

    // Clear existing mesh data
    ProcMeshComponent->ClearAllMeshSections();

    // Create mesh section for the bottom part
    TArray<FVector> BottomVertices;
    TArray<int32> BottomTriangles;
    GetBottomMeshData(BottomVertices, BottomTriangles);

    // Calculate the height of the bottom part
    float BottomHeight = 0.0f;
    for (const FVector& Vertex : BottomVertices)
    {
        BottomHeight = FMath::Max(BottomHeight, Vertex.Z);
    }

    // Offset the sphere vertices to sit on top of the bottom mesh
    for (FVector& Vertex : SphereVertices)
    {
        Vertex.Z += BottomHeight;
    }

    // Combine the vertices and triangles
    TArray<FVector> CombinedVertices = BottomVertices;
    CombinedVertices.Append(SphereVertices);

    TArray<int32> CombinedTriangles = BottomTriangles;
    for (int32 Triangle : SphereTriangles)
    {
        CombinedTriangles.Add(Triangle + BottomVertices.Num());
    }

    // Create the combined mesh section
    ProcMeshComponent->CreateMeshSection(0, CombinedVertices, CombinedTriangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), true);
}

void AVerticalRailActor::GetBottomMeshData(TArray<FVector>& Vertices, TArray<int32>& Triangles)
{
    // Define the size of the box
    float Width = 10.0f;
    float Depth = 10.0f;
    float Height = 100.0f;

    // Create the box
    CreateBox(Width, Depth, Height, Vertices, Triangles);
}

void AVerticalRailActor::GenerateBottomMesh()
{
    TArray<FVector> Vertices;
    TArray<int32> Triangles;

    // Define the size of the box
    float Width = 10.0f;
    float Depth = 10.0f;
    float Height = 100.0f;

    // Create the box
    CreateBox(Width, Depth, Height, Vertices, Triangles);

    // Clear existing mesh data
    ProcMeshComponent->ClearAllMeshSections();

    // Create mesh section for the bottom part
    ProcMeshComponent->CreateMeshSection(0, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), true);
}

void AVerticalRailActor::CreateBox(float Width, float Depth, float Height, TArray<FVector>& Vertices, TArray<int32>& Triangles)
{
    // Define vertices of the box
    Vertices.Add(FVector(0, 0, 0)); // 0
    Vertices.Add(FVector(Width, 0, 0)); // 1
    Vertices.Add(FVector(Width, Depth, 0)); // 2
    Vertices.Add(FVector(0, Depth, 0)); // 3

    Vertices.Add(FVector(0, 0, Height)); // 4
    Vertices.Add(FVector(Width, 0, Height)); // 5
    Vertices.Add(FVector(Width, Depth, Height)); // 6
    Vertices.Add(FVector(0, Depth, Height)); // 7

    // Define triangles for each face of the box (two triangles per face)

    // Bottom face
    Triangles.Add(0);
    Triangles.Add(2);
    Triangles.Add(1);
    Triangles.Add(0);
    Triangles.Add(3);
    Triangles.Add(2);

    // Top face
    Triangles.Add(4);
    Triangles.Add(5);
    Triangles.Add(6);
    Triangles.Add(4);
    Triangles.Add(6);
    Triangles.Add(7);

    // Front face
    Triangles.Add(0);
    Triangles.Add(1);
    Triangles.Add(5);
    Triangles.Add(0);
    Triangles.Add(5);
    Triangles.Add(4);

    // Back face
    Triangles.Add(2);
    Triangles.Add(3);
    Triangles.Add(7);
    Triangles.Add(2);
    Triangles.Add(7);
    Triangles.Add(6);

    // Left face
    Triangles.Add(0);
    Triangles.Add(4);
    Triangles.Add(7);
    Triangles.Add(0);
    Triangles.Add(7);
    Triangles.Add(3);

    // Right face
    Triangles.Add(1);
    Triangles.Add(2);
    Triangles.Add(6);
    Triangles.Add(1);
    Triangles.Add(6);
    Triangles.Add(5);
}

void AVerticalRailActor::CreateSphere(float Radius, int32 Segments, TArray<FVector>& Vertices, TArray<int32>& Triangles)
{
    Vertices.Empty();
    Triangles.Empty();

    for (int32 Latitude = 0; Latitude <= Segments; ++Latitude)
    {
        float Theta = Latitude * PI / Segments;
        float SinTheta = FMath::Sin(Theta);
        float CosTheta = FMath::Cos(Theta);

        for (int32 Longitude = 0; Longitude <= Segments; ++Longitude)
        {
            float Phi = Longitude * 2 * PI / Segments;
            float SinPhi = FMath::Sin(Phi);
            float CosPhi = FMath::Cos(Phi);

            FVector Vertex = FVector(
                Radius * SinTheta * CosPhi,
                Radius * SinTheta * SinPhi,
                Radius * CosTheta
            );

            Vertices.Add(Vertex);
        }
    }

    for (int32 Latitude = 0; Latitude < Segments; ++Latitude)
    {
        for (int32 Longitude = 0; Longitude < Segments; ++Longitude)
        {
            int32 First = (Latitude * (Segments + 1)) + Longitude;
            int32 Second = First + Segments + 1;

            Triangles.Add(First);
            Triangles.Add(Second);
            Triangles.Add(First + 1);

            Triangles.Add(Second);
            Triangles.Add(Second + 1);
            Triangles.Add(First + 1);
        }
    }
}

void AVerticalRailActor::CreateCone(float Height, float Radius, int32 Sides, TArray<FVector>& Vertices, TArray<int32>& Triangles)
{
    Vertices.Empty();
    Triangles.Empty();

    Vertices.Add(FVector(0, 0, Height)); // Apex

    for (int32 i = 0; i < Sides; ++i)
    {
        float Angle = i * 2 * PI / Sides;
        Vertices.Add(FVector(Radius * FMath::Cos(Angle), Radius * FMath::Sin(Angle), 0.0f));
    }

    for (int32 i = 0; i < Sides; ++i)
    {
        Triangles.Add(0);
        Triangles.Add((i + 1) % Sides + 1);
        Triangles.Add(i + 1);
    }

    // Base of the cone
    int32 BaseCenterIndex = Vertices.Add(FVector(0, 0, 0));
    for (int32 i = 0; i < Sides; ++i)
    {
        Triangles.Add(BaseCenterIndex);
        Triangles.Add(i + 1);
        Triangles.Add((i + 1) % Sides + 1);
    }
}

void AVerticalRailActor::CreatePyramid(float Height, float BaseRadius, TArray<FVector>& Vertices, TArray<int32>& Triangles)
{
    Vertices.Empty();
    Triangles.Empty();

    Vertices.Add(FVector(0, 0, Height)); // Apex

    Vertices.Add(FVector(-BaseRadius, -BaseRadius, 0));
    Vertices.Add(FVector(BaseRadius, -BaseRadius, 0));
    Vertices.Add(FVector(BaseRadius, BaseRadius, 0));
    Vertices.Add(FVector(-BaseRadius, BaseRadius, 0));

    // Sides
    Triangles.Add(0);
    Triangles.Add(1);
    Triangles.Add(2);

    Triangles.Add(0);
    Triangles.Add(2);
    Triangles.Add(3);

    Triangles.Add(0);
    Triangles.Add(3);
    Triangles.Add(4);

    Triangles.Add(0);
    Triangles.Add(4);
    Triangles.Add(1);

    // Base
    Triangles.Add(1);
    Triangles.Add(4);
    Triangles.Add(3);

    Triangles.Add(1);
    Triangles.Add(3);
    Triangles.Add(2);
}