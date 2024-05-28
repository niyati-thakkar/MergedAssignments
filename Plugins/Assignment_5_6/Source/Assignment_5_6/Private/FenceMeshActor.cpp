#include "FenceMeshActor.h"

#include "CylinderActor.h"
#include "Components/SplineComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AFenceMeshActor::AFenceMeshActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("USceneComponent"));
    RootComponent = SceneComponent;

    FenceSplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Fence Spline Component"));
    FenceSplineComponent->SetupAttachment(SceneComponent);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshAsset(TEXT("/Game/StarterContent/Architecture/Pillar_50x500.Pillar_50x500"));
    if (StaticMeshAsset.Succeeded())
    {
        SourceMesh = StaticMeshAsset.Object;

        static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("/Game/StarterContent/Materials/M_Basic_Wall"));
        if (MaterialAsset.Succeeded())
        {
            FenceMaterial = MaterialAsset.Object;
        }
    }

    PillarSpacing = 30.0f; // Default distance between pillars

    static ConstructorHelpers::FObjectFinder<UStaticMesh> BambooStickMeshAsset(TEXT("/Game/Megascans/3D_Assets/Bamboo_Stick_ud4pccafa/S_Bamboo_Stick_ud4pccafa_lod3_Var1.S_Bamboo_Stick_ud4pccafa_lod3_Var1"));
    if (BambooStickMeshAsset.Succeeded())
    {
        BambooStickMesh = BambooStickMeshAsset.Object;
    }

    static ConstructorHelpers::FObjectFinder<UDataTable> Fence_DT(TEXT("/Assignment_5_6/DataTable/Fence_DT.Fence_DT"));
    DataTable_Fence = Fence_DT.Object;

    CurrentFenceType = EFenceType::Random;
}

void AFenceMeshActor::ClearExistingPillars()
{
    TArray<UStaticMeshComponent*> Components;
    GetComponents<UStaticMeshComponent>(Components);

    for (UStaticMeshComponent* Component : Components)
    {
        if (Component && Component->GetName().StartsWith("Pillar"))
        {
            Component->DestroyComponent();
        }
    }
    SplineMeshes.Empty();
}

void AFenceMeshActor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    // Clear existing pillars and bamboo sticks to avoid duplication
    ClearExistingPillars();
    ClearExistingBambooSticks();

    int NumberOfSplinePoints = FenceSplineComponent->GetNumberOfSplinePoints();

    for (int i = 1; i < NumberOfSplinePoints; i++)
    {
        FVector StartPos = FenceSplineComponent->GetLocationAtSplinePoint(i - 1, ESplineCoordinateSpace::Local);
        FVector EndPos = FenceSplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);

        FVector Direction = (EndPos - StartPos).GetSafeNormal();
        float SegmentLength = FVector::Distance(StartPos, EndPos);
        int NumberOfPillars = FMath::FloorToInt(SegmentLength / PillarSpacing);
        float InternalSpacing = SegmentLength / NumberOfPillars;
        if (i != NumberOfSplinePoints - 1) NumberOfPillars--;
        // Create pillars
        for (int j = 0; j <= NumberOfPillars; ++j)
        {
            FVector PillarPos = StartPos + Direction * (j * InternalSpacing);

            // Create a new static mesh component for the pillar
            FString PillarName = FString::Printf(TEXT("Pillar_%d_%d"), i, j);
            UStaticMeshComponent* NewPillar = NewObject<UStaticMeshComponent>(this, FName(*PillarName));
            NewPillar->SetStaticMesh(SourceMesh);
            NewPillar->SetWorldLocation(PillarPos);
            NewPillar->SetupAttachment(SceneComponent);
            NewPillar->SetRelativeScale3D(FVector(0.15f)); // Adjust scale as needed
            NewPillar->SetMobility(EComponentMobility::Movable);
            if (FenceMaterial)
            {
                NewPillar->SetMaterial(0, FenceMaterial);
            }
            NewPillar->RegisterComponent();
            SplineMeshes.Add(NewPillar);
        }

        // Create bamboo stick at midpoint
        FVector BambooStickPos = StartPos + Direction * (SegmentLength / 2.0f);
        BambooStickPos.Z += 20;

        // Create a new static mesh component for the first bamboo stick
        FString BambooStickName1 = FString::Printf(TEXT("BambooStick_%d"), (i - 1) * 2);
        UStaticMeshComponent* NewBambooStick1 = NewObject<UStaticMeshComponent>(this, FName(*BambooStickName1));
        NewBambooStick1->SetStaticMesh(BambooStickMesh);
        NewBambooStick1->SetWorldLocation(BambooStickPos);
        NewBambooStick1->SetupAttachment(SceneComponent);
        NewBambooStick1->SetMobility(EComponentMobility::Movable);
        // Set material if needed
        NewBambooStick1->RegisterComponent();

        // Calculate rotation (if needed)
        FRotator StickRotation = Direction.Rotation(); // Rotate the stick along the spline direction
        NewBambooStick1->SetWorldRotation(StickRotation);
        NewBambooStick1->SetMaterial(0, FenceMaterial);
        // Set scale for the first bamboo stick
        FVector StickScale = FVector(((SegmentLength) / 145) , 0.25f, 0.25f); // Adjust scale as needed
        NewBambooStick1->SetWorldScale3D(StickScale);

        BambooStickPos.Z += 15;
        // Create a new static mesh component for the second bamboo stick
        FString BambooStickName2 = FString::Printf(TEXT("BambooStick_%d"), (i - 1) * 2 + 1);
        UStaticMeshComponent* NewBambooStick2 = NewObject<UStaticMeshComponent>(this, FName(*BambooStickName2));
        NewBambooStick2->SetStaticMesh(BambooStickMesh);
        NewBambooStick2->SetWorldLocation(BambooStickPos); // Adjust position as needed
        NewBambooStick2->SetupAttachment(SceneComponent);
        NewBambooStick2->SetMobility(EComponentMobility::Movable);
        // Set material if needed
        NewBambooStick2->RegisterComponent();
        NewBambooStick2->SetMaterial(0, FenceMaterial);
        // Calculate rotation (if needed)
        // Use the same rotation as the first bamboo stick
        NewBambooStick2->SetWorldRotation(StickRotation);

        // Set scale for the second bamboo stick
        // Use the same scale as the first bamboo stick
        NewBambooStick2->SetWorldScale3D(StickScale);
    }
}



// Called when the game starts or when spawned
void AFenceMeshActor::BeginPlay()
{
    Super::BeginPlay();

    ReplacePillarsWithRails();
    ReplaceBambooSticksWithProceduralMesh();
}

// Called every frame
void AFenceMeshActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}



void AFenceMeshActor::ReplacePillarsWithRails()
{


    DataTable_Fence->GetAllRows<FFenceTypes>(TEXT("FenceDataTable"), FenceRows);

    if (FenceRows.Num() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("FenceDataTable is empty!"));
        return;
    }

    for (UStaticMeshComponent* PillarComponent : SplineMeshes)
    {
        // Get the location of the pillar
        FVector PillarLocation = PillarComponent->GetComponentLocation();

        // Pick a random rail class from the data table rows
        //int32 RandomIndex = FMath::RandRange(0, FenceRows.Num() - 1);
        //TSubclassOf<AVerticalRailActor> RandomRailClass = FenceRows[RandomIndex]->Fence;

        //// Spawn an instance of AVerticalRailActor at the location of the pillar
        AVerticalRailActor* RailActor = GetWorld()->SpawnActor<AVerticalRailActor>(AVerticalRailActor::StaticClass(), PillarLocation, FRotator::ZeroRotator);
        RailActor->ActorType = CurrentFenceType;
        RailActor->InitialPillarGeneration();
        if (FenceMaterial)
        {
            UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(FenceMaterial, this);
            if (DynamicMaterial)
            {
                DynamicMaterial->SetScalarParameterValue(FName("TileY"), TileY);

                for (int i = 0; i < RailActor->ProcMeshComponent->GetNumSections(); i++)
                {
                    RailActor->ProcMeshComponent->SetMaterial(i, DynamicMaterial);
                }

            }
        }
        
        PillarComponent->DestroyComponent();
    }
    SplineMeshes.Empty();
}


void AFenceMeshActor::ClearExistingBambooSticks()
{
    TArray<UStaticMeshComponent*> Components;
    GetComponents<UStaticMeshComponent>(Components);

    for (UStaticMeshComponent* Component : Components)
    {
        // Check if the component is a bamboo stick
        if (Component && Component->GetStaticMesh() == BambooStickMesh)
        {
            Component->DestroyComponent();
        }
    }
}

void AFenceMeshActor::ReplaceBambooSticksWithProceduralMesh()
{
    TArray<UStaticMeshComponent*> Components;
    GetComponents<UStaticMeshComponent>(Components);

    for (UStaticMeshComponent* Component : Components)
    {
        if (Component && Component->GetStaticMesh() == BambooStickMesh)
        {
            FVector Location = Component->GetComponentLocation();
            FRotator Rotation = Component->GetComponentRotation();
            FVector Scale = Component->GetComponentScale();

            // Calculate the length of the bamboo stick
            float Length = Scale.X * Component->GetStaticMesh()->GetBoundingBox().GetSize().X;
            float Radius = 5.0f; // Adjust the radius as necessary

            // Spawn procedural mesh actor at the location of the bamboo stick
            ACylinderActor* CylinderActor = GetWorld()->SpawnActor<ACylinderActor>(Location, Rotation);
            if (CylinderActor)
            {
                // Optionally, set scale or other properties
                CylinderActor->GenerateCylinder(1.0f, Length, 20);
                //CylinderActor->SetActorScale3D(Scale);

                if (FenceMaterial)
                {
                    UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(FenceMaterial, this);
                    if (DynamicMaterial)
                    {
                        DynamicMaterial->SetScalarParameterValue(FName("TileX"), Length / 100.0f); // Example scaling
                        DynamicMaterial->SetScalarParameterValue(FName("TileY"), TileY);

                        for(int i=0;i< CylinderActor->ProcMeshComponent->GetNumSections();i++)
                        {
                            CylinderActor->ProcMeshComponent->SetMaterial(i, DynamicMaterial);
                        }
                        
                    }
                }
            }
            

            // Destroy the bamboo stick
            Component->DestroyComponent();
        }
    }
}
