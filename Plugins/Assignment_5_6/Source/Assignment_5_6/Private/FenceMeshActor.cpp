#include "FenceMeshActor.h"

#include "CylinderActor.h"
#include "Components/SplineComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AFenceMeshActor::AFenceMeshActor()
{
    PrimaryActorTick.bCanEverTick = true;

    SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("USceneComponent"));
    RootComponent = SceneComponent;

    FenceSplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Fence Spline Component"));
    FenceSplineComponent->SetupAttachment(SceneComponent);

    //static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshAsset(TEXT("/Game/StarterContent/Architecture/Pillar_50x500.Pillar_50x500"));
    //if (StaticMeshAsset.Succeeded())
    //{
    //    SourceMesh = StaticMeshAsset.Object;

    //    static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("/Game/StarterContent/Materials/M_Basic_Wall"));
    //    if (MaterialAsset.Succeeded())
    //    {
    //        MaterialType = MaterialAsset.Object;
    //    }
    //}

    //PillarSpacing = 30.0f; // Default distance between pillars

    static ConstructorHelpers::FObjectFinder<UStaticMesh> HorizontalMeshAsset(TEXT("/Assignment_5_6/StaticMeshes/SM_HorizontalMesh.SM_HorizontalMesh"));
    if (HorizontalMeshAsset.Succeeded())
    {
        HorizontalMesh = HorizontalMeshAsset.Object;
    }

    static ConstructorHelpers::FObjectFinder<UFenceDataAsset> Fence_DA(TEXT("/Assignment_5_6/DataAsset/Fence_UDA.Fence_UDA"));
    FenceData = Fence_DA.Object;

    FenceType = EFenceType::Random;
    MaterialType = EMaterialType::Random;
    FenceProperty = {10.0f, 50.0f, 40.0f, 2 };

}

void AFenceMeshActor::GenerateVerticalMesh()
{
    if (FenceSplineComponent == nullptr) return;

    int32 NumberOfSplinePoints = FenceSplineComponent->GetNumberOfSplinePoints();

    for (int32 i = 0; i < NumberOfSplinePoints - 1; ++i)
    {
        FVector StartPos = FenceSplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
        FVector EndPos = FenceSplineComponent->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);
        float SegmentLength = FVector::Dist(StartPos, EndPos);

        int32 NumberOfPillars = FMath::FloorToInt(SegmentLength / FenceProperty.Spacing);
        float InternalSpacing = SegmentLength / (NumberOfPillars > 0 ? NumberOfPillars + 1 : 1); // +1 to include both endpoints

        for (int32 j = 0; j <= NumberOfPillars; ++j)
        {
            FVector PillarPos = FMath::Lerp(StartPos, EndPos, j / float(NumberOfPillars + 1));
            FRotator PillarRot = FenceSplineComponent->GetRotationAtDistanceAlongSpline(FenceSplineComponent->GetDistanceAlongSplineAtSplinePoint(i) + j * InternalSpacing, ESplineCoordinateSpace::Local);

            EFenceType CurrentFenceType = FenceType;
            EMaterialType CurrentMaterialType = MaterialType;
            AVerticalRailActor::SelectForRandom(CurrentFenceType, CurrentMaterialType);

            SourceMesh = FenceData->FenceMap[CurrentFenceType].MeshClass;
            SourceMaterial = FenceData->Materials[CurrentMaterialType];

            // Create a new static mesh component for the pillar
            FString PillarName = FString::Printf(TEXT("Pillar_%d_%d"), i, j);
            UStaticMeshComponent* NewPillar = NewObject<UStaticMeshComponent>(this, FName(*PillarName));
            NewPillar->SetStaticMesh(SourceMesh);

            NewPillar->SetRelativeLocationAndRotation(PillarPos, PillarRot);
            NewPillar->SetupAttachment(SceneComponent);

            NewPillar->SetRelativeScale3D(FVector(FenceProperty.SideLength/5, FenceProperty.SideLength/5, FenceProperty.BottomHeight / 50.0f));
            NewPillar->SetMobility(EComponentMobility::Movable);

            if (SourceMaterial)
            {
                for (int32 k = 0; k < NewPillar->GetNumMaterials(); k++)
                {
                    NewPillar->SetMaterial(k, SourceMaterial);
                }
            }

            NewPillar->RegisterComponent();
            SplineMeshes.Add(NewPillar);
        }
    }

    // Ensure there's a pillar at the last spline point
    FVector LastPillarPos = FenceSplineComponent->GetLocationAtSplinePoint(NumberOfSplinePoints - 1, ESplineCoordinateSpace::Local);
    FRotator LastPillarRot = FenceSplineComponent->GetRotationAtSplinePoint(NumberOfSplinePoints - 1, ESplineCoordinateSpace::Local);

    EFenceType CurrentFenceType = FenceType;
    EMaterialType CurrentMaterialType = MaterialType;
    AVerticalRailActor::SelectForRandom(CurrentFenceType, CurrentMaterialType);

    SourceMesh = FenceData->FenceMap[CurrentFenceType].MeshClass;
    SourceMaterial = FenceData->Materials[CurrentMaterialType];

    FString LastPillarName = FString::Printf(TEXT("Pillar_%d_%d"), NumberOfSplinePoints - 1, 0);
    UStaticMeshComponent* LastPillar = NewObject<UStaticMeshComponent>(this, FName(*LastPillarName));
    LastPillar->SetStaticMesh(SourceMesh);

    LastPillar->SetRelativeLocationAndRotation(LastPillarPos, LastPillarRot);
    LastPillar->SetupAttachment(SceneComponent);

    LastPillar->SetRelativeScale3D(FVector(FenceProperty.SideLength / 5.0f, FenceProperty.SideLength/ 5.0f, FenceProperty.BottomHeight / 50.0f));
    LastPillar->SetMobility(EComponentMobility::Movable);

    if (SourceMaterial)
    {
        for (int32 k = 0; k < LastPillar->GetNumMaterials(); k++)
        {
            LastPillar->SetMaterial(k, SourceMaterial);
        }
    }

    LastPillar->RegisterComponent();
    SplineMeshes.Add(LastPillar);
}



void AFenceMeshActor::ClearExistingMeshes()
{
    TArray<UStaticMeshComponent*> Components;
    GetComponents<UStaticMeshComponent>(Components);

    for (UStaticMeshComponent* Component : Components)
    {
        if(Component)
        {
            Component->DestroyComponent();
        }

    }
    SplineMeshes.Empty();

}

void AFenceMeshActor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    ClearExistingMeshes();

    GenerateVerticalMesh();
    GenerateHorizontalMesh();
    
}
void AFenceMeshActor::GenerateHorizontalMesh()
{
    int NumberOfSplinePoints = FenceSplineComponent->GetNumberOfSplinePoints();

    for (int i = 1; i < NumberOfSplinePoints; i++)
    {
        FenceSplineComponent->SetSplinePointType(i - 1, ESplinePointType::Linear);
        FVector StartPos = FenceSplineComponent->GetLocationAtSplinePoint(i - 1, ESplineCoordinateSpace::Local);
        FVector EndPos = FenceSplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local); // Fixed to World space for consistency
        FRotator StartRotation = FenceSplineComponent->GetRotationAtSplinePoint(i - 1, ESplineCoordinateSpace::Local);


        FVector Direction = (EndPos - StartPos).GetSafeNormal();
        float SegmentLength = FVector::Distance(StartPos, EndPos);




        // Create bamboo stick at midpoint
        FVector HorizontalMeshPos = StartPos + Direction * (SegmentLength / 2.0f);
        HorizontalMeshPos.Z += FenceProperty.BottomHeight / 3;

        // Create a new static mesh component for the first bamboo stick
        FString HorizontalMeshName1 = FString::Printf(TEXT("HorizontalMesh_%d"), (i - 1) * 2);
        UStaticMeshComponent* NewHorizontalMesh1 = NewObject<UStaticMeshComponent>(this, FName(*HorizontalMeshName1));
        NewHorizontalMesh1->SetStaticMesh(HorizontalMesh);
        NewHorizontalMesh1->SetWorldLocation(HorizontalMeshPos);
        NewHorizontalMesh1->SetupAttachment(SceneComponent);
        NewHorizontalMesh1->SetMobility(EComponentMobility::Movable);
        NewHorizontalMesh1->RegisterComponent();

        // Calculate rotation and scale for the first bamboo stick
        FRotator StickRotation = Direction.Rotation(); // Rotate the stick along the spline direction
        FVector StickScale = FVector((SegmentLength) / 10.0f, FenceProperty.HorizontalRadius, FenceProperty.HorizontalRadius); // Adjust scale as needed
        NewHorizontalMesh1->SetWorldRotation(StickRotation);
        NewHorizontalMesh1->SetWorldScale3D(StickScale);

        // Adjust position for the second bamboo stick
        HorizontalMeshPos.Z += FenceProperty.BottomHeight / 3;

        // Create a new static mesh component for the second bamboo stick
        FString HorizontalMeshName2 = FString::Printf(TEXT("HorizontalMesh_%d"), (i - 1) * 2 + 1);
        UStaticMeshComponent* NewHorizontalMesh2 = NewObject<UStaticMeshComponent>(this, FName(*HorizontalMeshName2));
        NewHorizontalMesh2->SetStaticMesh(HorizontalMesh);
        NewHorizontalMesh2->SetWorldLocation(HorizontalMeshPos);
        NewHorizontalMesh2->SetupAttachment(SceneComponent);
        NewHorizontalMesh2->SetMobility(EComponentMobility::Movable);
        NewHorizontalMesh2->RegisterComponent();

        // Use the same rotation and scale as the first bamboo stick
        NewHorizontalMesh2->SetWorldRotation(StickRotation);
        NewHorizontalMesh2->SetWorldScale3D(StickScale);
        if (SourceMaterial)
        {
            NewHorizontalMesh1->SetMaterial(0, SourceMaterial);
            NewHorizontalMesh2->SetMaterial(0, SourceMaterial);
        }
    }
}

// Called when the game starts or when spawned
void AFenceMeshActor::BeginPlay()
{
    Super::BeginPlay();

    ReplacePillarsWithRails();
   ReplaceHorizontalMeshsWithProceduralMesh();
}

// Called every frame
void AFenceMeshActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}



void AFenceMeshActor::ReplacePillarsWithRails()
{


 /*   DataTable_Fence->GetAllRows<FFenceTypes>(TEXT("FenceDataTable"), FenceRows);

    if (FenceRows.Num() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("FenceDataTable is empty!"));
        return;
    }*/

    for (UStaticMeshComponent* PillarComponent : SplineMeshes)
    {
        // Get the location of the pillar
        FVector PillarLocation = PillarComponent->GetComponentLocation();

        // Pick a random rail class from the data table rows
        //int32 RandomIndex = FMath::RandRange(0, FenceRows.Num() - 1);
        //TSubclassOf<AVerticalRailActor> RandomRailClass = FenceRows[RandomIndex]->Fence;

        //// Spawn an instance of AVerticalRailActor at the location of the pillar
        EFenceType CurrentFenceType = FenceType;
        EMaterialType CurrentMaterialType = MaterialType;
        AVerticalRailActor::SelectForRandom(CurrentFenceType, CurrentMaterialType);

        if (FenceData && FenceData->FenceMap.Contains(CurrentFenceType))
        {
            TSubclassOf<AActor> FenceClass = FenceData->FenceMap[CurrentFenceType].ActorClass;
            if (FenceClass)
            {
                // Spawn the fence actor at the actor's location
                AVerticalRailActor* RailActor = GetWorld()->SpawnActor<AVerticalRailActor>(FenceClass, PillarLocation, GetActorRotation());
                //RailActor->FenceType = CurrentFenceType;
                RailActor->FenceProperty = FenceProperty;
                RailActor->InitialPillarGeneration();

                if (SourceMaterial)
                {
                    UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(FenceData->Materials[CurrentMaterialType], this);
                    if (DynamicMaterial)
                    {
                        DynamicMaterial->SetScalarParameterValue(FName("TileY"), TileY);

                        for (int i = 0; i < RailActor->ProcMeshComponent->GetNumSections(); i++)
                        {
                            RailActor->ProcMeshComponent->SetMaterial(i, DynamicMaterial);
                        }

                    }
                }

            
        }
        }
        
        PillarComponent->DestroyComponent();
    }
    SplineMeshes.Empty();
}
//
void AFenceMeshActor::ReplaceHorizontalMeshsWithProceduralMesh()
{
    TArray<UStaticMeshComponent*> Components;
    GetComponents<UStaticMeshComponent>(Components);

    for (UStaticMeshComponent* Component : Components)
    {
        if (Component && Component->GetStaticMesh() == HorizontalMesh)
        {
            FVector Location = Component->GetComponentLocation();
            FRotator Rotation = Component->GetComponentRotation();
            FVector Scale = Component->GetComponentScale();

            // Calculate the length of the bamboo stick
            float Length = Scale.X * Component->GetStaticMesh()->GetBoundingBox().GetSize().X;
            

            // Spawn procedural mesh actor at the location of the bamboo stick
            ACylinderActor* CylinderActor = GetWorld()->SpawnActor<ACylinderActor>(Location, Rotation);
            if (CylinderActor)
            {
                CylinderActor->Radius = FenceProperty.HorizontalRadius;
                CylinderActor->Height = Length;
                CylinderActor->Segments = 30;
                // Optionally, set scale or other properties
                CylinderActor->GenerateCylinder();
                //CylinderActor->SetActorScale3D(Scale);

                if (SourceMaterial)
                {
                    UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(SourceMaterial, this);
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
