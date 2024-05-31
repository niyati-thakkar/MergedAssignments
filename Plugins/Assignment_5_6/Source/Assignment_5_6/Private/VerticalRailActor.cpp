// Fill out your copyright notice in the Description page of Project Settings.


#include "VerticalRailActor.h"

#include "Kismet/KismetMathLibrary.h"

// Sets default values
AVerticalRailActor::AVerticalRailActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProcMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProcMeshComponent"));
	RootComponent = ProcMeshComponent;

    static ConstructorHelpers::FObjectFinder<UFenceDataAsset> Fence_DA(TEXT("/Assignment_5_6/DataAsset/Fence_UDA.Fence_UDA"));
    FenceData = Fence_DA.Object;

    FenceType = EFenceType::Random;
    MaterialType = EMaterialType::Random;


    FenceProperty.Spacing = 20;
    FenceProperty.SideLength = 5;
    FenceProperty.BottomHeight = 50;

}

// Called when the game starts or when spawned
void AVerticalRailActor::BeginPlay()
{
	Super::BeginPlay();

//    InitialPillarGeneration();
	
}

// Called every frame
void AVerticalRailActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AVerticalRailActor::SelectForRandom(EFenceType& FType, EMaterialType& EType)
{
    if (FType == EFenceType::Random)
    {

        int32 RandomIndex = FMath::RandRange(0, 5);
        switch (RandomIndex)
        {
        case 0:
        {
            FType = EFenceType::WindsorTurnedCapital;
            break;
        }
        case 1:
        {
            FType = EFenceType::RoundTurnedCapital;
            break;
        }
        case 2:
        {
            FType = EFenceType::ACornCapital;
            break;
        }
        case 3:
        {
            FType = EFenceType::RoundedOverTopCapital;
            break;
        }
        case 4:
        {
            FType = EFenceType::GothicStarCapital;
            break;
        }
        case 5:
        {
            FType = EFenceType::PyramidTop;
            break;
        }

        }
    }

    if(EType == EMaterialType::Random)
    {
        int32 RandomIndex = FMath::RandRange(0, 3);
        switch (RandomIndex)
        {
        case 0:
        {
            EType = EMaterialType::Oak_Wood;
            break;
        }
        case 1:
        {
            EType = EMaterialType::Pine_Wood;
            break;
        }
        case 2:
        {
            EType = EMaterialType::Walnut_Floor_Wood;
            break;
        }
        case 3:
        {
            EType = EMaterialType::Walnut_Wood;
            break;
        }
        }
        
    }
}

void AVerticalRailActor::InitialPillarGeneration()
{
    TopMeshLerpValue = 0.0f;
    //MeshTypeIndex = 0;
    Location = FVector::ZeroVector;
    Location.Z += FenceProperty.BottomHeight/2;

    ProcMeshComponent->ClearAllMeshSections();
    Segment = 0;
    SelectForRandom(FenceType, MaterialType);
    
    GenerateCube(FVector(FenceProperty.SideLength * 2, FenceProperty.SideLength * 2, FenceProperty.BottomHeight));

    Location.Z += FenceProperty.BottomHeight / 2 + FenceProperty.SideLength*0.1;

    GenerateCube(FVector(FenceProperty.SideLength));

    Location.Z += FenceProperty.SideLength / 2;

    GenerateCube(FVector(FenceProperty.SideLength * 2, FenceProperty.SideLength * 2, FenceProperty.SideLength));

	Location.Z += FenceProperty.SideLength / 2;

    
    switch (FenceType)
    {
    case EFenceType::WindsorTurnedCapital:
    {
        Location.Z += FenceProperty.SideLength / 2; 

        WindsorTurnedCapital();
        break;
    }
    case EFenceType::RoundTurnedCapital:
    {
        Location.Z += FenceProperty.SideLength / 2; // Adjusted to place the rounded top at the top of the cube
        RoundTurnedCapital();

        break;
        
    }
    case EFenceType::ACornCapital:
    {
        Location.Z += FenceProperty.SideLength / 2; // Adjusted to place the rounded top at the top of the cube

        ACornCapital();
        break;
    }
    case EFenceType::RoundedOverTopCapital:
    {
        
        RoundedOverTopCapital();
        break;
    }
    case EFenceType::GothicStarCapital:
    {
        Location.Z += FenceProperty.SideLength / 2; // Adjusted to place the rounded top at the top of the cube

        GothicStarCapital();
        break;
    }
    case EFenceType::PyramidTop:
    {
        
        PyramidTop();
        break;
    }
    
    
    

    }
    for(int i = 0; i < Segment; i++)
    {
        ProcMeshComponent->SetMaterial(i, FenceData->Materials[MaterialType]);
    }
	
}
void AVerticalRailActor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    
    InitialPillarGeneration();

}

void AVerticalRailActor::RoundTurnedCapital()
{

    GenerateBellShape(FenceProperty.SideLength, FenceProperty.SideLength /2, FenceProperty.SideLength /2, 1, 10, 10);
    Location.Z += FenceProperty.SideLength;

	GenerateSphere(FenceProperty.SideLength, 20, 20);
}
void AVerticalRailActor::WindsorTurnedCapital()
{

    GenerateBellShape(FenceProperty.SideLength, FenceProperty.SideLength / 2, FenceProperty.SideLength / 2, 1, 10, 10);
    Location.Z += FenceProperty.SideLength;

    GenerateSphere(FenceProperty.SideLength, 20, 20);
    Location.Z += FenceProperty.SideLength + FenceProperty.SideLength / 8;
    GenerateBellShape(FenceProperty.SideLength / 2, FenceProperty.SideLength / 4, 0.5, 1, 10, 10);

}
void AVerticalRailActor::PyramidTop()
{

    GeneratePyramid(FenceProperty.SideLength *2, FenceProperty.SideLength *2);
}
void AVerticalRailActor::ACornCapital()
{

    GenerateBellShape(FenceProperty.SideLength, FenceProperty.SideLength / 2, FenceProperty.SideLength / 2, 1, 10, 10);
    Location.Z += FenceProperty.SideLength *2-2;

    GenerateCornShape(20, FenceProperty.SideLength, FenceProperty.SideLength, FenceProperty.SideLength *5);
    Location.Z -= FenceProperty.SideLength *4 -2;
    GenerateTorus(FenceProperty.SideLength, FenceProperty.SideLength +2, 20, 20);

}
void AVerticalRailActor::GothicStarCapital()
{
    GenerateBellShape(FenceProperty.SideLength, FenceProperty.SideLength / 2, FenceProperty.SideLength / 2, 1, 10, 10);
    Location.Z += FenceProperty.SideLength /2;

    GeneratePyramid(FenceProperty.SideLength *2, FenceProperty.SideLength *2);

}
void AVerticalRailActor::RoundedOverTopCapital()
{

    GenerateFenceTop(FenceProperty.SideLength, FenceProperty.SideLength * 2, FenceProperty.SideLength *2);
}


void AVerticalRailActor::GenerateCube(FVector Dimensions)
{
    // Setup mesh buffers
    const int32 VertexCount = 6 * 4; // 6 sides on a cube, 4 verts each
    const int32 TriangleCount = 6 * 2 * 3; // 2x triangles per cube side, 3 verts each
    TArray<FVector> Positions;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FProcMeshTangent> Tangents;
    TArray<FVector2D> TexCoords;
    Positions.SetNum(VertexCount);
    Triangles.SetNum(TriangleCount);
    Normals.SetNum(VertexCount);
    Tangents.SetNum(VertexCount);
    TexCoords.SetNum(VertexCount);

    // Calculate a half offset so we get correct center of object
    const float OffsetX = Dimensions.X / 2.0f;
    const float OffsetY = Dimensions.Y / 2.0f;
    const float OffsetZ = Dimensions.Z / 2.0f;

    // Define the 8 corners of the cube
    const FVector P0 = FVector(OffsetX, OffsetY, -OffsetZ);
    const FVector P1 = FVector(OffsetX, -OffsetY, -OffsetZ);
    const FVector P2 = FVector(OffsetX, -OffsetY, OffsetZ);
    const FVector P3 = FVector(OffsetX, OffsetY, OffsetZ);
    const FVector P4 = FVector(-OffsetX, OffsetY, -OffsetZ);
    const FVector P5 = FVector(-OffsetX, -OffsetY, -OffsetZ);
    const FVector P6 = FVector(-OffsetX, -OffsetY, OffsetZ);
    const FVector P7 = FVector(-OffsetX, OffsetY, OffsetZ);

    int32 VertexOffset = 0;
    int32 TriangleOffset = 0;

    // Front (+X) face: 0-1-2-3
    BuildQuad(Positions, Triangles, Normals, Tangents, TexCoords, P0, P1, P2, P3, VertexOffset, TriangleOffset, FVector(1, 0, 0), FProcMeshTangent(0, 1, 0));

    // Back (-X) face: 5-4-7-6
    BuildQuad(Positions, Triangles, Normals, Tangents, TexCoords, P5, P4, P7, P6, VertexOffset, TriangleOffset, FVector(-1, 0, 0), FProcMeshTangent(0, -1, 0));

    // Left (-Y) face: 1-5-6-2
    BuildQuad(Positions, Triangles, Normals, Tangents, TexCoords, P1, P5, P6, P2, VertexOffset, TriangleOffset, FVector(0, -1, 0), FProcMeshTangent(1, 0, 0));

    // Right (+Y) face: 4-0-3-7
    BuildQuad(Positions, Triangles, Normals, Tangents, TexCoords, P4, P0, P3, P7, VertexOffset, TriangleOffset, FVector(0, 1, 0), FProcMeshTangent(-1, 0, 0));

    // Top (+Z) face: 6-7-3-2
    BuildQuad(Positions, Triangles, Normals, Tangents, TexCoords, P6, P7, P3, P2, VertexOffset, TriangleOffset, FVector(0, 0, 1), FProcMeshTangent(0, 1, 0));

    // Bottom (-Z) face: 1-0-4-5
    BuildQuad(Positions, Triangles, Normals, Tangents, TexCoords, P1, P0, P4, P5, VertexOffset, TriangleOffset, FVector(0, 0, -1), FProcMeshTangent(0, -1, 0));

    ProcMeshComponent->CreateMeshSection_LinearColor(Segment++, Positions, Triangles, Normals, TexCoords, TArray<FLinearColor>(), Tangents, true);
}

void AVerticalRailActor::BuildQuad(TArray<FVector>& InVertices, TArray<int32>& InTriangles, TArray<FVector>& InNormals, TArray<FProcMeshTangent>& InTangents, TArray<FVector2D>& InTexCoords, const FVector BottomLeft, const FVector BottomRight, const FVector TopRight, const FVector TopLeft, int32& VertexOffset, int32& TriangleOffset, const FVector Normal, const FProcMeshTangent Tangent)
{
    const int32 Index1 = VertexOffset++;
    const int32 Index2 = VertexOffset++;
    const int32 Index3 = VertexOffset++;
    const int32 Index4 = VertexOffset++;
    InVertices[Index1] = BottomLeft + Location;
    InVertices[Index2] = BottomRight + Location;
    InVertices[Index3] = TopRight + Location;
    InVertices[Index4] = TopLeft + Location;
    InTexCoords[Index1] = FVector2D(0.0f, 1.0f);
    InTexCoords[Index2] = FVector2D(1.0f, 1.0f);
    InTexCoords[Index3] = FVector2D(1.0f, 0.0f);
    InTexCoords[Index4] = FVector2D(0.0f, 0.0f);
    InTriangles[TriangleOffset++] = Index1;
    InTriangles[TriangleOffset++] = Index2;
    InTriangles[TriangleOffset++] = Index3;
    InTriangles[TriangleOffset++] = Index1;
    InTriangles[TriangleOffset++] = Index3;
    InTriangles[TriangleOffset++] = Index4;
    // On a cube side, all the vertex normals face the same way
    InNormals[Index1] = InNormals[Index2] = InNormals[Index3] = InNormals[Index4] = Normal;
    InTangents[Index1] = InTangents[Index2] = InTangents[Index3] = InTangents[Index4] = Tangent;
}

void AVerticalRailActor::GenerateSphere(float Radius, int32 Segments, int32 Rings)
{
    //ProcMeshComponent->ClearAllMeshSections();

    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UVs;
    TArray<FProcMeshTangent> Tangents;
    TArray<FLinearColor> Colors;
    for (int r = 0; r < Rings; ++r)
    {
        float Phi = PI * r / (Rings + 1);
        for (int s = 0; s < Segments; ++s)
        {
            float Theta = 2 * PI * s / Segments;
            FVector Pos = FVector(Radius * FMath::Sin(Phi) * FMath::Cos(Theta), Radius * FMath::Sin(Phi) * FMath::Sin(Theta), Radius * FMath::Cos(Phi)) + Location;
            Vertices.Add(Pos);
            Normals.Add(Pos.GetSafeNormal());
            UVs.Add(FVector2D(static_cast<float>(s) / Segments, static_cast<float>(r) / (Rings + 1)));
            FVector Tangent = FVector(-FMath::Sin(Theta), FMath::Cos(Theta), 0).GetSafeNormal();
            Tangents.Add(FProcMeshTangent(Tangent, false));
            Colors.Add(FLinearColor(1, 1, 1, 1));

            // We only add extra vertices at the seam on the first ring iteration
            if (s == Segments)
            {
                // Duplicate the first vertex of each ring at the seam for seamless UVs
                Vertices.Add(Vertices[1 + (r - 1) * (Segments + 1)]);
                Normals.Add(Normals[1 + (r - 1) * (Segments + 1)]);
                UVs.Add(FVector2D(1.f, static_cast<float>(r) / (Rings + 1)));
                Tangents.Add(Tangents[1 + (r - 1) * (Segments + 1)]);
                Colors.Add(FLinearColor(1, 1, 1, 1));
            }
        }
    }

    // Bottom vertex
    Vertices.Add(Location - FVector(0, 0, Radius));
    Normals.Add(FVector(0, 0, -1));
    UVs.Add(FVector2D(0.5f, 1));
    Tangents.Add(FProcMeshTangent(FVector(1, 0, 0), false));
    Colors.Add(FLinearColor(1, 1, 1, 1));

    // Top Cap
    for (int s = 0; s < Segments; ++s)
    {
        Triangles.Add(0);
        Triangles.Add(1 + s);
        Triangles.Add(1 + (s + 1) % Segments);
    }

    // Body
    for (int r = 0; r < Rings; ++r)
    {
        for (int s = 0; s < Segments; ++s)
        {
            int Current = 1 + r * Segments + s;
            int Next = Current + 1;
            if (s == Segments - 1) Next = 1 + r * Segments;  // Wrap around

            int Below = Current + Segments;
            int BelowNext = Next + Segments;

            Triangles.Add(Current);
            Triangles.Add(Next);
            Triangles.Add(BelowNext);

            Triangles.Add(Current);
            Triangles.Add(BelowNext);
            Triangles.Add(Below);
        }
    }

    // Bottom Cap
    int BottomIndex = Vertices.Num() - 1;
    int FirstOfLastRing = 1 + (Rings - 1) * Segments;
    for (int s = 0; s < Segments; ++s)
    {
        Triangles.Add(BottomIndex);
        Triangles.Add(FirstOfLastRing + (s + 1) % Segments);
        Triangles.Add(FirstOfLastRing + s);
    }

    ProcMeshComponent->CreateMeshSection_LinearColor(Segment++, Vertices, Triangles, Normals, UVs, Colors, Tangents, true);
}




void AVerticalRailActor::GenerateCone(float Radius, float Height, int32 Segments)
{
   /// ProcMeshComponent->ClearAllMeshSections();
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UVs;
    TArray<FProcMeshTangent> Tangents;
    TArray<FLinearColor> Colors;

    // Generate the vertices, normals, UVs, and tangents for the cone
    Vertices.Add(Location + FVector(0.0f, 0.0f, Height));
    Normals.Add(FVector(0.0f, 0.0f, 1.0f));
    UVs.Add(FVector2D(0.5f, 1.0f));
    Tangents.Add(FProcMeshTangent(FVector(1.0f, 0.0f, 0.0f), false));

    for (int32 s = 0; s <= Segments; s++)
    {
        float Angle = static_cast<float>(s) / Segments * 2.0f * PI;
        float X = Radius * FMath::Cos(Angle);
        float Y = Radius * FMath::Sin(Angle);

        Vertices.Add(Location + FVector(X, Y, 0.0f));
        Normals.Add(FVector(FMath::Cos(Angle), FMath::Sin(Angle), 0.0f));
        UVs.Add(FVector2D(static_cast<float>(s) / Segments, 0.0f));

        FVector TangentX = FVector(-FMath::Sin(Angle), FMath::Cos(Angle), 0.0f);
        FVector TangentY = FVector(FMath::Cos(Angle), FMath::Sin(Angle), 0.0f);
        FVector TangentZ = FVector(0.0f, 0.0f, 1.0f);

        Tangents.Add(FProcMeshTangent(TangentX, false));
    }

    // Generate the triangle indices
    for (int32 s = 0; s < Segments; s++)
    {
        Triangles.Add(0);
        Triangles.Add(s + 2);
        Triangles.Add(s + 1);
    }

    // Generate the vertex colors
    for (int32 i = 0; i < Vertices.Num(); i++)
    {
        Colors.Add(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
    }

    // Add the mesh to the procedural mesh component
   ProcMeshComponent->CreateMeshSection_LinearColor(Segment++, Vertices, Triangles, Normals, UVs, Colors, Tangents, true);
}



void AVerticalRailActor::GenerateBellShape(float BaseRadius, float Height, float RimRadius, float CurvatureFactor, int NumSlices, int NumStacks)
{
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UVs;
    TArray<FProcMeshTangent> Tangents;
    TArray<FLinearColor> Colors;

    float Theta, Phi;
    float DeltaTheta = 2.0f * PI / NumSlices;
    float DeltaPhi = PI / (NumStacks - 1);

    // Generate vertices
    for (int i = 0; i < NumStacks; i++)
    {
        Phi = i * DeltaPhi;

        // Bell profile function for radius interpolation with curvature
        float t = Phi / (PI / 2.0f);
        float CurrentRadius = FMath::Lerp(BaseRadius, RimRadius, FMath::Sin(t * CurvatureFactor));

        for (int j = 0; j < NumSlices; j++)
        {
            Theta = j * DeltaTheta;

            float X = CurrentRadius * FMath::Sin(Theta);
            float Y = CurrentRadius * FMath::Cos(Theta);
            float Z = Height * (t - 1.0f); // Z decreases with height for bell shape

            // Apply location offset
            X += Location.X;
            Y += Location.Y;
            Z += Location.Z;

            Vertices.Add(FVector(X, Y, Z));

            // Calculate normals for outward-facing orientation
            Normals.Add(FVector(X - Location.X, Y - Location.Y, Z - Location.Z).GetSafeNormal());

            // UV mapping
            UVs.Add(FVector2D((float)j / (NumSlices - 1), (float)i / (NumStacks - 1)));

            // Tangents (optional)
            Tangents.Add(FProcMeshTangent(-FMath::Sin(Theta), FMath::Cos(Theta), 0.0f));
        }
    }

    // Generate triangles (quads converted to triangles)
    for (int i = 0; i < NumStacks - 1; i++)
    {
        for (int j = 0; j < NumSlices - 1; j++)
        {
            int BottomLeft = i * NumSlices + j;
            int BottomRight = BottomLeft + 1;
            int TopLeft = BottomLeft + NumSlices;
            int TopRight = TopLeft + 1;

            // First triangle (bottom left to top left to top right)
            Triangles.Add(BottomLeft);
            Triangles.Add(BottomRight);
            Triangles.Add(TopLeft);

            // Second triangle (top left to top right to bottom right)
            Triangles.Add(TopLeft);
            Triangles.Add(BottomRight);
            Triangles.Add(TopRight);
        }

        // Handle wrap-around triangle for each stack
        int BottomLeftWrap = i * NumSlices + (NumSlices - 1);
        int BottomRightWrap = i * NumSlices;
        int TopLeftWrap = BottomLeftWrap + NumSlices;
        int TopRightWrap = BottomRightWrap + NumSlices;

        // First triangle (bottom left to top left to bottom right)
        Triangles.Add(BottomLeftWrap);
        Triangles.Add(BottomRightWrap);
        Triangles.Add(TopLeftWrap);

        // Second triangle (top left to bottom right to top right)
        Triangles.Add(TopLeftWrap);
        Triangles.Add(BottomRightWrap);
        Triangles.Add(TopRightWrap);
    }

    // Assign colors (optional, can be white for now)
    for (int i = 0; i < Vertices.Num(); i++)
    {
        Colors.Add(FLinearColor::White);
    }

    // Clear any existing mesh data
    // ProcMeshComponent->ClearAllMeshSections();

    ProcMeshComponent->CreateMeshSection_LinearColor(Segment++, Vertices, Triangles, Normals, UVs, Colors, Tangents, true);
}




void AVerticalRailActor::GenerateTorus(float InnerRadius, float OuterRadius, int32 RadialSegments, int32 TubularSegments)
{
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UVs;
    TArray<FProcMeshTangent> Tangents;
    TArray<FLinearColor> VertexColors;

    float Radius = (OuterRadius - InnerRadius) / 2.0f;
    float CentreRadius = InnerRadius + Radius;

    for (int32 RadialIndex = 0; RadialIndex <= RadialSegments; ++RadialIndex)
    {
        float RadialFraction = (float)RadialIndex / (float)RadialSegments;
        float RadialAngle = RadialFraction * PI * 2.0f;

        FVector RadialPosition = FVector(CentreRadius * FMath::Cos(RadialAngle), CentreRadius * FMath::Sin(RadialAngle), 0.0f);

        for (int32 TubularIndex = 0; TubularIndex <= TubularSegments; ++TubularIndex)
        {
            float TubularFraction = (float)TubularIndex / (float)TubularSegments;
            float TubularAngle = TubularFraction * PI * 2.0f;

            FVector Normal = FVector(FMath::Cos(TubularAngle) * FMath::Cos(RadialAngle), FMath::Cos(TubularAngle) * FMath::Sin(RadialAngle), FMath::Sin(TubularAngle));
            FVector VertexPosition = RadialPosition + Normal * Radius + Location;

            Vertices.Add(VertexPosition);
            Normals.Add(Normal);
            UVs.Add(FVector2D(RadialFraction, TubularFraction));
            Tangents.Add(FProcMeshTangent(-FMath::Sin(TubularAngle), FMath::Cos(TubularAngle), 0.0f));
            VertexColors.Add(FLinearColor::White);
        }
    }

    for (int32 RadialIndex = 0; RadialIndex < RadialSegments; ++RadialIndex)
    {
        for (int32 TubularIndex = 0; TubularIndex < TubularSegments; ++TubularIndex)
        {
            int32 First = (RadialIndex * (TubularSegments + 1)) + TubularIndex;
            int32 Second = First + TubularSegments + 1;

            Triangles.Add(First);
            Triangles.Add(Second);
            Triangles.Add(First + 1);

            Triangles.Add(Second);
            Triangles.Add(Second + 1);
            Triangles.Add(First + 1);
        }
    }

    ProcMeshComponent->CreateMeshSection_LinearColor(Segment++, Vertices, Triangles, Normals, UVs, VertexColors, Tangents, true);
}

void AVerticalRailActor::GenerateSemiEggShape(int32 NumSegments, float RadiusX, float RadiusY, float Height, TArray<FVector>& Vertices, TArray<int32>& Triangles, TArray<FVector>& Normals, TArray<FVector2D>& UVs, TArray<FProcMeshTangent>& Tangents, TArray<FLinearColor>& VertexColors)
{
    for (int32 i = 0; i <= NumSegments; i++)
    {
        float Theta = i * PI / NumSegments; // From 0 to PI
        for (int32 j = 0; j <= NumSegments; j++)
        {
            float Phi = j * 2.0f * PI / NumSegments; // From 0 to 2*PI

            float x = RadiusX * FMath::Sin(Theta) * FMath::Cos(Phi) + Location.X;
            float y = RadiusY * FMath::Sin(Theta) * FMath::Sin(Phi) + Location.Y;
            float z = Height * FMath::Cos(Theta) + Location.Z;

            Vertices.Add(FVector(x, y, z));

            // Calculate normal for each vertex
            FVector Normal = FVector(FMath::Sin(Theta) * FMath::Cos(Phi), FMath::Sin(Theta) * FMath::Sin(Phi), FMath::Cos(Theta));
            Normals.Add(Normal);

            // Calculate UV for each vertex
            UVs.Add(FVector2D((float)j / (float)NumSegments, (float)i / (float)NumSegments));

            // Add tangent
            Tangents.Add(FProcMeshTangent(-FMath::Sin(Phi), FMath::Cos(Phi), 0.0f));

            // Add vertex color
            VertexColors.Add(FLinearColor::White);
        }
    }

    // Generate triangles
    for (int32 i = 0; i < NumSegments; i++)
    {
        for (int32 j = 0; j < NumSegments; j++)
        {
            int32 First = (i * (NumSegments + 1)) + j;
            int32 Second = First + NumSegments + 1;

            Triangles.Add(First);
            Triangles.Add(Second);
            Triangles.Add(First + 1);

            Triangles.Add(Second);
            Triangles.Add(Second + 1);
            Triangles.Add(First + 1);
        }
    }
}

void AVerticalRailActor::GenerateInvertedSemiEggShape(int32 NumSegments, float RadiusX, float RadiusY, float Height, TArray<FVector>& Vertices, TArray<int32>& Triangles, TArray<FVector>& Normals, TArray<FVector2D>& UVs, TArray<FProcMeshTangent>& Tangents, TArray<FLinearColor>& VertexColors)
{
    for (int32 i = 0; i <= NumSegments; i++)
    {
        float Theta = i * PI / NumSegments; // From 0 to PI
        for (int32 j = 0; j <= NumSegments; j++)
        {
            float Phi = j * 2.0f * PI / NumSegments; // From 0 to 2*PI

            float x = RadiusX * FMath::Sin(Theta) * FMath::Cos(Phi) + Location.X;
            float y = RadiusY * FMath::Sin(Theta) * FMath::Sin(Phi) + Location.Y;
            float z = -Height * FMath::Cos(Theta) + Location.Z;

            Vertices.Add(FVector(x, y, z));

            // Calculate normal for each vertex
            FVector Normal = FVector(FMath::Sin(Theta) * FMath::Cos(Phi), FMath::Sin(Theta) * FMath::Sin(Phi), -FMath::Cos(Theta));
            Normals.Add(Normal);

            // Calculate UV for each vertex
            UVs.Add(FVector2D((float)j / (float)NumSegments, (float)i / (float)NumSegments));

            // Add tangent
            Tangents.Add(FProcMeshTangent(-FMath::Sin(Phi), FMath::Cos(Phi), 0.0f));

            // Add vertex color
            VertexColors.Add(FLinearColor::White);
        }
    }

    // Generate triangles
    for (int32 i = 0; i < NumSegments; i++)
    {
        for (int32 j = 0; j < NumSegments; j++)
        {
            int32 First = (i * (NumSegments + 1)) + j;
            int32 Second = First + NumSegments + 1;

            Triangles.Add(First);
            Triangles.Add(Second);
            Triangles.Add(First + 1);

            Triangles.Add(Second);
            Triangles.Add(Second + 1);
            Triangles.Add(First + 1);
        }
    }
}
void AVerticalRailActor::GenerateCornShape(int32 NumSegments, float BaseRadiusX, float BaseRadiusY, float Height)
{
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UVs;
    TArray<FProcMeshTangent> Tangents;
    TArray<FLinearColor> VertexColors;

    // Bottom section (thicker and shorter semi-egg shape with tapered bottom)
    float CornBottomHeight = Height * 0.3f; // Adjust factor for desired bottom height
    float BottomRadiusXFactor = 1.2f; // Adjust factor for bottom radius increase
    float BottomRadiusYFactor = 1.1f; // Adjust factor for bottom radius increase (can be different from X)
    float BottomRadiusX = BaseRadiusX * BottomRadiusXFactor;
    float BottomRadiusY = BaseRadiusY * BottomRadiusYFactor;
    float BottomTaper = 0.1f; // Adjust for desired bottom taper (0 for no taper)

    GenerateSemiEggShape(NumSegments, BottomRadiusX, BottomRadiusY, CornBottomHeight, Vertices, Triangles, Normals, UVs, Tangents, VertexColors);

    // Middle section (elliptical cylinder with rounded ends)
    float MiddleRadiusX = (BaseRadiusX + BottomRadiusX) / 2.0f; // Adjust for smooth transition
    float MiddleRadiusY = MiddleRadiusX * 0.8f; // Adjust for ellipticity
    float MiddleHeight = Height * 0.4f; // Adjust factor for desired middle height
    float MiddleSegmentHeight = MiddleHeight / (NumSegments - 2); // Segment height for cylinder

    // Generate vertices for top and bottom circles (accounting for ellipticity)
    float AngleStep = 2.0f * PI / NumSegments;
    for (int32 i = 0; i <= NumSegments; i++)
    {
        float Angle = i * AngleStep;
        float CosAngle = FMath::Cos(Angle);
        float SinAngle = FMath::Sin(Angle);

        float TopX = MiddleRadiusX * CosAngle + Location.X;
        float TopY = MiddleRadiusY * SinAngle + Location.Y;
        float TopZ = Location.Z + Height - MiddleHeight;

        float BottomX = (MiddleRadiusX * (1.0f - BottomTaper)) * CosAngle + Location.X;
        float BottomY = (MiddleRadiusY * (1.0f - BottomTaper)) * SinAngle + Location.Y;
        float BottomZ = Location.Z + CornBottomHeight;

        Vertices.Add(FVector(TopX, TopY, TopZ));
        Vertices.Add(FVector(BottomX, BottomY, BottomZ));

        // Normals for top and bottom circles (point outwards)
        Normals.Add(FVector(CosAngle, SinAngle, 0.0f));
        Normals.Add(FVector(CosAngle, SinAngle, 0.0f));

        // UVs for top and bottom circles (circular mapping)
        UVs.Add(FVector2D((float)i / (float)NumSegments, 1.0f));
        UVs.Add(FVector2D((float)i / (float)NumSegments, 0.0f));

        // Tangents (optional, adjust based on cylinder orientation)
        Tangents.Add(FProcMeshTangent(-SinAngle, CosAngle, 0.0f));
        Tangents.Add(FProcMeshTangent(-SinAngle, CosAngle, 0.0f));

        // Vertex colors (consider a gradient or noise for variation)
        VertexColors.Add(FLinearColor(1.0f, 0.9f, 0.7f, 1.0f));
        VertexColors.Add(FLinearColor(1.0f, 0.8f, 0.0f, 1.0f));
    }

    // Curved surface vertices (using additional calculations for elliptical cylinder)
    for (int32 i = 0; i < NumSegments - 2; i++) // Iterate through segments excluding top and bottom
    {
        float Angle = (i + 1) * AngleStep;
        float CosAngle = FMath::Cos(Angle);
        float SinAngle = FMath::Sin(Angle);

        float YOffset = MiddleSegmentHeight * (i + 1); // Adjust Y offset for each segment

        // Vertices for curved surface with ellipticity
        float CurvedX = MiddleRadiusX * CosAngle + Location.X;
        float CurvedY = MiddleRadiusY * SinAngle + Location.Y;
        float CurvedZ = Location.Z + CornBottomHeight + YOffset;
        Vertices.Add(FVector(CurvedX, CurvedY, CurvedZ));

        // Normals for curved surface (point outwards)
        FVector Normal = FVector(CosAngle * MiddleRadiusX, SinAngle * MiddleRadiusY, 0.0f);
        Normal.Normalize();
        Normals.Add(Normal);

        // UVs for curved surface (linear mapping along the height)
        UVs.Add(FVector2D((float)i / (float)(NumSegments - 3), 0.0f)); // Adjust for segment count

        // Tangents (optional, adjust based on curved surface orientation)
        Tangents.Add(FProcMeshTangent(-SinAngle, CosAngle, 0.0f));

        // Vertex colors (consider a gradient or noise for variation)
        VertexColors.Add(FLinearColor(1.0f, 0.9f, 0.7f, 1.0f));
    }

    // Generate triangles for the middle section (cylinder walls and curved surface)
    int32 MiddleOffset = (NumSegments + 2) * 2; // Offset for middle section vertices

    for (int32 i = 0; i < NumSegments - 3; i++)
    {
        int32 First = i * 2;
        int32 Second = First + 1;
        int32 Third = (i + 1) * 2 + MiddleOffset;
        int32 Fourth = Third + 1;

        // Cylinder wall triangles
        Triangles.Add(First);
        Triangles.Add(Third);
        Triangles.Add(Second);

        Triangles.Add(Second);
        Triangles.Add(Third);
        Triangles.Add(Fourth);
    }

    // Top section (thinner inverted semi-egg shape)
    float TopHeight = Height * 0.3f; // Adjust factor for desired top height
    float TopRadiusXFactor = 0.8f; // Adjust factor for top radius decrease
    float TopRadiusYFactor = 0.9f; // Adjust factor for top radius decrease (can be different from X)
    float TopRadiusX = BaseRadiusX * TopRadiusXFactor;
    float TopRadiusY = BaseRadiusY * TopRadiusYFactor;
    float TopTaper = 0.1f; // Adjust for desired top taper (0 for no taper)

    // Adjust location for top section
    Location.Z += Height - TopHeight;

    GenerateInvertedSemiEggShape(NumSegments, TopRadiusX, TopRadiusY, TopHeight, Vertices, Triangles, Normals, UVs, Tangents, VertexColors);


    // Create mesh sections
    ProcMeshComponent->CreateMeshSection_LinearColor(Segment++, Vertices, Triangles, Normals, UVs, VertexColors, Tangents, true);
}
void AVerticalRailActor::GeneratePyramid(float BaseLength, float Height)
{
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UVs;
    TArray<FProcMeshTangent> Tangents;
    TArray<FLinearColor> VertexColors;

    // Calculate half length for convenience
    float HalfLength = BaseLength * 0.5f;

    // Vertices for the base square
    FVector BottomLeft = FVector(-HalfLength, -HalfLength, 0.0f) + Location;
    FVector BottomRight = FVector(HalfLength, -HalfLength, 0.0f) + Location;
    FVector TopRight = FVector(HalfLength, HalfLength, 0.0f) + Location;
    FVector TopLeft = FVector(-HalfLength, HalfLength, 0.0f) + Location;

    // Apex vertex
    FVector Apex = Location + FVector(0.0f, 0.0f, Height);

    // Add base vertices
    Vertices.Add(BottomLeft);
    Vertices.Add(BottomRight);
    Vertices.Add(TopRight);
    Vertices.Add(TopLeft);

    // Add apex vertex
    Vertices.Add(Apex);

    // Normals for base vertices (pointing downwards)
    FVector BaseNormal = FVector(0.0f, 0.0f, -1.0f);
    Normals.Add(BaseNormal);
    Normals.Add(BaseNormal);
    Normals.Add(BaseNormal);
    Normals.Add(BaseNormal);

    // Normals for side faces
    FVector SideNormal0 = FVector::CrossProduct(BottomRight - BottomLeft, Apex - BottomLeft).GetSafeNormal();
    FVector SideNormal1 = FVector::CrossProduct(TopRight - BottomRight, Apex - BottomRight).GetSafeNormal();
    FVector SideNormal2 = FVector::CrossProduct(TopLeft - TopRight, Apex - TopRight).GetSafeNormal();
    FVector SideNormal3 = FVector::CrossProduct(BottomLeft - TopLeft, Apex - TopLeft).GetSafeNormal();

    // Normals for apex vertex (each face contributes one normal)
    Normals.Add(SideNormal0);
    Normals.Add(SideNormal1);
    Normals.Add(SideNormal2);
    Normals.Add(SideNormal3);
    Normals.Add(SideNormal0 + SideNormal1 + SideNormal2 + SideNormal3); // Averaged normal for the apex

    // UVs for base vertices (simple planar mapping)
    UVs.Add(FVector2D(0.0f, 0.0f));
    UVs.Add(FVector2D(1.0f, 0.0f));
    UVs.Add(FVector2D(1.0f, 1.0f));
    UVs.Add(FVector2D(0.0f, 1.0f));

    // UV for apex vertex (center of the texture)
    UVs.Add(FVector2D(0.5f, 0.5f));

    // Tangents (optional, adjust based on orientation)
    for (int32 i = 0; i < 5; ++i)
    {
        Tangents.Add(FProcMeshTangent(1.0f, 0.0f, 0.0f));
    }

    // Vertex colors (can be white for now)
    for (int32 i = 0; i < 5; ++i)
    {
        VertexColors.Add(FLinearColor::White);
    }

    // Base triangles
    Triangles.Add(0); Triangles.Add(2); Triangles.Add(1);
    Triangles.Add(0); Triangles.Add(3); Triangles.Add(2);

    // Side triangles
    Triangles.Add(0); Triangles.Add(1); Triangles.Add(4);
    Triangles.Add(1); Triangles.Add(2); Triangles.Add(4);
    Triangles.Add(2); Triangles.Add(3); Triangles.Add(4);
    Triangles.Add(3); Triangles.Add(0); Triangles.Add(4);

    // Create mesh section
    ProcMeshComponent->CreateMeshSection_LinearColor(Segment++, Vertices, Triangles, Normals, UVs, VertexColors, Tangents, true);
}

void AVerticalRailActor::GenerateFenceTop(float Radius, float Length, float Width)
{

    //Mesh->ClearAllMeshSections();
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UVs;
    TArray<FProcMeshTangent> Tangents;
    TArray<FLinearColor> VertexColors;

    // Calculate points for semicircles
    const int32 NumSegments = 20; // Number of segments in the semicircle
    const float DeltaTheta = 180.0f / (NumSegments - 1);

    Vertices.Add(FVector{ 0,0,0 } + Location);
    Vertices.Add(FVector{ 0,  Width / 2,0 } + Location);
    Vertices.Add(FVector{ 0,-Width / 2,0 } + Location);

    UVs.Add(FVector2D(0.5f, 0.5f)); // Center
    UVs.Add(FVector2D(0.5f, 0.5f)); // Top middle (on front)
    UVs.Add(FVector2D(0.5f, 0.0f)); // Top middle (on back)


    // Top vertices of the semicircles
    for (int32 i = 0; i < NumSegments; i++)
    {
        float AngleRad = FMath::DegreesToRadians(i * DeltaTheta);
        float X = Radius * FMath::Cos(AngleRad);
        float Z = Radius * FMath::Sin(AngleRad);

        // Vertices on Front
        Vertices.Add(FVector(X, Width / 2, Z) + Location);

        // Vertices on Back
        Vertices.Add(FVector(X, -Width / 2, Z) + Location);


        // Compute UVs for each vertex
        float U = 0.5f + (X / (2 * Radius)); // Normalize X to [0, 1]
        float V_Front = 0.5f + (Z / (2 * Radius)); // Normalize Z to [0, 1] for front
        float V_Back = 0.5f - (Z / (2 * Radius)); // Normalize Z to [0, 1] for back

        UVs.Add(FVector2D(U, V_Front)); // UV for front vertices
        UVs.Add(FVector2D(U, V_Back));  // UV for back vertices


    }

    // Add triangles for semicircles (front faces)
    {
        int32 CurrentVertexIndex = 3;

        for (int32 i = 0; i < NumSegments - 1; i++)
        {
            Triangles.Add(1);
            Triangles.Add(CurrentVertexIndex);
            Triangles.Add(CurrentVertexIndex + 2);
            CurrentVertexIndex += 2;
        }
    }


    // Add triangles for semicircles (Back faces)

    {
        int32 CurrentVertexIndex = 4;

        for (int32 i = 0; i < NumSegments - 1; i++)
        {
            Triangles.Add(2);
            Triangles.Add(CurrentVertexIndex + 2);
            Triangles.Add(CurrentVertexIndex);
            CurrentVertexIndex += 2;
        }
    }


    Normals.Add({ 0,0,-1 });
    Normals.Add({ 0,1,0 });
    Normals.Add({ 0,-1,0 });


    // Calculate normals and tangents
    for (int32 i = 3; i < Vertices.Num(); i++)
    {
        if (i % 2 != 0)
        {

            Normals.Add({ 0,1,0 });

        }
        else
        {

            Normals.Add({ 0, -1 ,0 });
        }


    }


    {
        int32 CurrentVertexIndex = 3;

        for (int32 i = 0; i < (NumSegments - 1); i++)
        {
            Triangles.Add(CurrentVertexIndex);
            Triangles.Add(CurrentVertexIndex + 1);
            Triangles.Add(CurrentVertexIndex + 2);

            Triangles.Add(CurrentVertexIndex + 2);
            Triangles.Add(CurrentVertexIndex + 1);
            Triangles.Add(CurrentVertexIndex + 3);


            CurrentVertexIndex += 2;
        }
    }


    auto OldTotalVertices = Vertices.Num();

    for (int i = 3; i < OldTotalVertices; ++i)
    {
        auto Vertex = Vertices[i];
        Vertices.Add(Vertex);

        auto UV = UVs[i];

        UVs.Add(UV);

    }


    {
        int32 CurrentVertexIndex = OldTotalVertices;

        for (int32 i = 0; i < OldTotalVertices - 3; i++)
        {
            Triangles.Add(CurrentVertexIndex);
            Triangles.Add(CurrentVertexIndex + 1);
            Triangles.Add(CurrentVertexIndex + 2);

            Triangles.Add(CurrentVertexIndex + 2);
            Triangles.Add(CurrentVertexIndex + 1);
            Triangles.Add(CurrentVertexIndex + 3);


            CurrentVertexIndex += 2;
        }
    }


    {
        int32 CurrentVertexIndex = OldTotalVertices;
        // Calculate normals and tangents
        for (int32 i = 0; i < OldTotalVertices - 3; i++)
        {
            Normals.Add(UKismetMathLibrary::GetDirectionUnitVector(Vertices[0], Vertices[CurrentVertexIndex]));
            CurrentVertexIndex++;
        }
    }



    // Create mesh section
    ProcMeshComponent->CreateMeshSection_LinearColor(Segment++, Vertices, Triangles, Normals, UVs, TArray<FLinearColor>(), Tangents, true);
}


