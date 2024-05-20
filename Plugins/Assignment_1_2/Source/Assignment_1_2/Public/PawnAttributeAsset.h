// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PawnAttributeAsset.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENT_1_2_API UPawnAttributeAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Asset Properties")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Asset Properties")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Asset Properties")
	float JumpHeight;
};
