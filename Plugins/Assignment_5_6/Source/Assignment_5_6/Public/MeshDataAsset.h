// Fill out your copyright notice in the Description page of Project Settings.

#pragma once



#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MeshDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENT_5_6_API UMeshDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UStaticMesh*> StaticMeshes;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UMaterialInstance*> ToonShaderMaterials;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator MinRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator MaxRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxDistance;
		

	
};
