// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "GameFramework/Actor.h"
#include "CylinderActor.generated.h"

UCLASS()
class ASSIGNMENT_5_6_API ACylinderActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACylinderActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void OnConstruction(const FTransform& Transform) override;

	UFUNCTION(BlueprintCallable, Category = Defauls)
	void GenerateCylinder(float Radius, float Height, int32 Segments);

	UFUNCTION()
	void AddCylinderCaps(TArray<FVector>& Vertices,
		TArray<int32>& Triangles,
		TArray<FVector>& Normals,
		TArray<FVector2D>& UVs,
		TArray<FLinearColor>& Colors,
		TArray<FProcMeshTangent>& Tangents,
		float Radius,
		float HalfHeight,
		int32 Segments);

	UProceduralMeshComponent* ProcMeshComponent;
};
