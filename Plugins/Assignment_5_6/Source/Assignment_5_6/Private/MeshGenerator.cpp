 // Fill out your copyright notice in the Description page of Project Settings.


#include "MeshGenerator.h"

#include "A5PlayerController.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AMeshGenerator::AMeshGenerator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UMeshDataAsset> Mesh_UDA(TEXT("/Assignment_5_6/DataAsset/Mesh_UDA.Mesh_UDA"));
	if (Mesh_UDA.Succeeded())
	{
		MeshDataAsset = Mesh_UDA.Object;
	}


}

// Called when the game starts or when spawned
void AMeshGenerator::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		// Assuming the selection area is an actor in the world
		for (TActorIterator<ASelectionArea> It(GetWorld()); It; ++It)
		{
			SelectionArea = *It;
			if (SelectionArea)
			{
				break;
			}
		}
	}

}

// Called every frame
void AMeshGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMeshGenerator::ScatterObjects()
{
	for (auto& Pair : HISMComponents)
	{
		if (UHierarchicalInstancedStaticMeshComponent* HISM = Pair.Value)
		{


			AsyncTask(ENamedThreads::GameThread, [this, HISM]()
				{
					HISM->ClearInstances();
				});

		}
	}

	/*SlowTask = new FScopedSlowTask(NumberOfInstances, FText::FromString("Scattering Objects"));
	SlowTask->MakeDialog(true);*/
	if (AsyncScatterTask && !AsyncScatterTask->IsDone())
	{
		AsyncScatterTask->EnsureCompletion();  
		delete AsyncScatterTask;
		AsyncScatterTask = nullptr;
	}
	AsyncScatterTask = new FAsyncTask< FAsyncScatterTask>(this);
	AsyncScatterTask->StartBackgroundTask();
}

void AMeshGenerator::AddInstance(UStaticMesh* StaticMesh, FTransform Transform)
{
	UHierarchicalInstancedStaticMeshComponent** HISMCPtr = HISMComponents.Find(StaticMesh);
	if (HISMCPtr && *HISMCPtr && (*HISMCPtr)->IsValidLowLevel())
	{


		(*HISMCPtr)->AddInstance(Transform);


	}
	else
	{
		UHierarchicalInstancedStaticMeshComponent* NewHISMCPtr = NewObject< UHierarchicalInstancedStaticMeshComponent>(this);

		int RandomIndex = FMath::RandRange(0, MeshDataAsset->ToonShaderMaterials.Num() - 1);
		NewHISMCPtr->SetStaticMesh(StaticMesh);
		NewHISMCPtr->SetMaterial(0, MeshDataAsset->ToonShaderMaterials[RandomIndex]);
		HISMComponents.Add(StaticMesh, NewHISMCPtr);
		NewHISMCPtr->RegisterComponentWithWorld(GetWorld());
		NewHISMCPtr->AddInstance(Transform);
		
	}
	
		
		

		

		
			
}

void AMeshGenerator::UpdateProgress(float Progress)
{
	AsyncTask(ENamedThreads::GameThread, [this, Progress]()
		{
			ScatterProgressUpdated.Broadcast(Progress);
		});
}
void AMeshGenerator::ManageProgressBar(bool makeVisible)
{
	AsyncTask(ENamedThreads::GameThread, [this, makeVisible]()
		{
			APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			if (PlayerController)
			{
				AA5PlayerController* A5PC = Cast<AA5PlayerController>(PlayerController);
				if (A5PC)
				{
					if(makeVisible)
					{
						A5PC->ShowProgressPanel();
					}else
					{
						A5PC->HideProgressPanel();
					}
					
				}

			}
		});
}

//void AMeshGenerator::FinishScatter()
//{
//	/*if (SlowTask)
//	{
//		SlowTask->Destroy();
//	}*/
//
//}