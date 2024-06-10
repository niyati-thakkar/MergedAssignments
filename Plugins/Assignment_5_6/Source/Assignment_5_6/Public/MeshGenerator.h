// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AsyncScatterTask.h"
#include "MeshDataAsset.h"
#include "ProceduralMeshComponent.h"
#include "Misc/ScopedSlowTask.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "SelectionArea.h"
#include "MeshGenerator.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScatterProgressUpdated, float, ScatterProgress);

UCLASS()
class ASSIGNMENT_5_6_API AMeshGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMeshGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;


	FAsyncTask<FAsyncScatterTask>* AsyncScatterTask;
	
	FOnScatterProgressUpdated ScatterProgressUpdated;

	UFUNCTION(BlueprintCallable)
	void ScatterObjects();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMeshDataAsset* MeshDataAsset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NumberOfInstances;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<UStaticMesh*, UHierarchicalInstancedStaticMeshComponent*> HISMComponents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ASelectionArea* SelectionArea;

	UFUNCTION(BlueprintCallable)
	void AddInstance(UStaticMesh* StaticMesh, FTransform Transforms);
	void UpdateProgress(float Progress);
	UFUNCTION(BlueprintCallable)
	void ManageProgressBar(bool makeVisible);


	//UFUNCTION(BlueprintCallable)
	//void FinishScatter();
};
