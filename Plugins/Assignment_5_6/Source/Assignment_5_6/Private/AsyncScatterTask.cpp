#include "AsyncScatterTask.h"
#include "MeshGenerator.h"
#include "ProceduralMeshComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "SelectionArea.h"

FAsyncScatterTask::FAsyncScatterTask(AMeshGenerator* InScatterActor)
{
    if (InScatterActor) {
        ScatterActor = InScatterActor;
    }

}

void FAsyncScatterTask::DoWork()
{
    if(ScatterActor->IsValidLowLevel() && IsValid(ScatterActor))
    {
        ScatterActor->ManageProgressBar(true);
    }
    
    if (ScatterActor->IsValidLowLevel() && IsValid(ScatterActor))
    {
        if (ScatterActor->MeshDataAsset && ScatterActor->SelectionArea)
        {
            const TArray<UStaticMesh*>& StaticMeshes = ScatterActor->MeshDataAsset->StaticMeshes;
            const float& MinScale = ScatterActor->MeshDataAsset->MinScale;
            const float& MaxScale = ScatterActor->MeshDataAsset->MaxScale;
            const FRotator& MinRotation = ScatterActor->MeshDataAsset->MinRotation;
            const FRotator& MaxRotation = ScatterActor->MeshDataAsset->MaxRotation;
            const float& MinDistance = ScatterActor->MeshDataAsset->MinDistance;
            const float Padding = 10.0f; // Example padding value

            
            for (int i = 0; ScatterActor->IsValidLowLevel() && IsValid(ScatterActor) && i < ScatterActor->NumberOfInstances; i++)
            {
                int RandomIndex = FMath::RandRange(0, StaticMeshes.Num() - 1);
                UStaticMesh* CurrentMesh = StaticMeshes[RandomIndex];

                FVector Position;

                bool bValidPosition = false;

                if (ScatterActor->SelectionArea->bIsSphere)
                {
                    FVector Center = ScatterActor->SelectionArea->GetActorLocation();
                    float Radius = ScatterActor->SelectionArea->Radius - Padding;

                    int iterator = 0;
                    do
                    {
                        Position = Center + FMath::RandPointInBox(FBox(FVector(-Radius), FVector(Radius)));
                    } while ((Position - Center).SizeSquared() > Radius * Radius && iterator++ < 20);

                    bValidPosition = true;
                }
                else
                {
                    FVector BoundingExtent = ScatterActor->SelectionArea->Dimensions / 2;
                    FVector Origin = ScatterActor->SelectionArea->GetActorLocation();
                    FBox BoundingBox(Origin - BoundingExtent, Origin + BoundingExtent);

                    Position = FMath::RandPointInBox(BoundingBox);
                    //Position.Z = Origin.Z - BoundingExtent.Z;
                }

                // Generate random scale and rotation
                FVector RandomScale = FVector(FMath::FRandRange(MinScale, MaxScale));
                FRotator RandomRotation = FMath::Lerp(MinRotation, MaxRotation, FMath::FRand());

                // Add the instance with the random position, scale, and rotation
                FTransform Transform(RandomRotation, Position, RandomScale);
                AsyncTask(ENamedThreads::GameThread, [this, i,CurrentMesh, Transform]()
                    {
                        if(ScatterActor->IsValidLowLevel() && IsValid(ScatterActor))
                        {
                            ScatterActor->AddInstance(CurrentMesh, Transform);

                            float Progress = ((float)(i + 1) / static_cast<float>(ScatterActor->NumberOfInstances));
                            ScatterActor->UpdateProgress(Progress);
                        }
                        

                    });
                if(ScatterActor->IsValidLowLevel() && IsValid(ScatterActor) && ScatterActor->NumberOfInstances < 10000){
                    FPlatformProcess::Sleep((float)2 / static_cast<float>(ScatterActor->NumberOfInstances));
                }else
                {
                    FPlatformProcess::Sleep(0.001f);
                }
                
                
                
                //
            }
            
            
            
        }

         
    }
    if (ScatterActor->IsValidLowLevel() && IsValid(ScatterActor)) {
        ScatterActor->ManageProgressBar(false);
    }
}

