// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VerticalRailActor.h"
#include "Components/SplineMeshComponent.h"
#include "GameFramework/Actor.h"
#include "FenceMeshActor.generated.h"


USTRUCT()
struct FFenceProperties
{
	GENERATED_BODY()

	UPROPERTY()
	float length;

	UPROPERTY()
	float width;

	UPROPERTY()
	float height;

	UPROPERTY()
	float spacing;
};

UCLASS()
class ASSIGNMENT_5_6_API AFenceMeshActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFenceMeshActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	USplineMeshComponent* FenceSplineComponent;

	UPROPERTY()
	TSubclassOf < AVerticalRailActor > VerticalRailActorClass;
};
