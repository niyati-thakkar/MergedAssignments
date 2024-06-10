// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectionArea.h"

#include "Kismet/GameplayStatics.h"


// Sets default values
ASelectionArea::ASelectionArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    Location = FVector::ZeroVector;
    ProcMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProcMeshComponent"));
    RootComponent = ProcMeshComponent;
    bIsSphere = true;
    bIsSpawned = false;

    static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialRef(TEXT("/DatasmithContent/Materials/FBXImporter/VRED/Glass.Glass"));
    if (MaterialRef.Succeeded())
    {
        TranslucentMaterial = MaterialRef.Object;
    }
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> DefaultMaterialRef( TEXT("/Engine/EngineMaterials/DefaultMaterial.DefaultMaterial"));
    if(DefaultMaterialRef.Succeeded())
    {
        DefaultMaterial = DefaultMaterialRef.Object;
    }
	
    
}

// Called when the game starts or when spawned
void ASelectionArea::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASelectionArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    if(!bIsSpawned)
    {
        UpdatePosition();
    }
        
}

void ASelectionArea::UpdatePosition()
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    if (PlayerController)
    {
        FVector MouseLocation, MouseDirection;
        PlayerController->DeprojectMousePositionToWorld(MouseLocation, MouseDirection);
        FCollisionQueryParams QueryParams;
        QueryParams.bTraceComplex = true;
        QueryParams.AddIgnoredActor(this);
        FHitResult HitResult;
    	FVector TraceEnd = MouseLocation + (MouseDirection * 10000.f);
        if(GetWorld()->LineTraceSingleByChannel(HitResult, MouseLocation, TraceEnd, ECC_Visibility, QueryParams))
        {
            LastMouseLocation = HitResult.Location;
            SpawnGeneratedMesh();
        }

        LastMouseLocation = HitResult.Location;
    }
}

void ASelectionArea::SpawnGeneratedMesh()
{
    
    if (bIsSphere)
    {
        LastMouseLocation.Z += Radius;

    }
    else
    {
        LastMouseLocation.Z += Dimensions.Z / 2;
    }
    SetActorLocation(LastMouseLocation);
    if (bIsSphere)
    {
        LastMouseLocation.Z -= Radius;

    }
    else
    {
        LastMouseLocation.Z -= Dimensions.Z / 2;
    }

    if(bIsSpawned && TranslucentMaterial)
    {
            ProcMeshComponent->SetMaterial(0, TranslucentMaterial);
        
    }
    if(!bIsSpawned && DefaultMaterial && ProcMeshComponent->GetMaterial(0) != DefaultMaterial)
    {
        ProcMeshComponent->SetMaterial(0, DefaultMaterial);
    }
}

void ASelectionArea::GenerateCube()
{
    bIsSphere = false;
    ProcMeshComponent->ClearAllMeshSections();
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

    ProcMeshComponent->CreateMeshSection_LinearColor(0, Positions, Triangles, Normals, TexCoords, TArray<FLinearColor>(), Tangents, true);
	
}

void ASelectionArea::BuildQuad(TArray<FVector>& InVertices, TArray<int32>& InTriangles, TArray<FVector>& InNormals, TArray<FProcMeshTangent>& InTangents, TArray<FVector2D>& InTexCoords, const FVector BottomLeft, const FVector BottomRight, const FVector TopRight, const FVector TopLeft, int32& VertexOffset, int32& TriangleOffset, const FVector Normal, const FProcMeshTangent Tangent)
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

void ASelectionArea::GenerateSphere()
{
    bIsSphere = true;

    int32 Segments = 50;
    int32 Rings = 50;
    ProcMeshComponent->ClearAllMeshSections();

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

    ProcMeshComponent->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, Colors, Tangents, true);
    
}
