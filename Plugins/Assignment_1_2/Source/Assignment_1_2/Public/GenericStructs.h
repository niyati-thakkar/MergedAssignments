// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericEnums.h"
#include "Templates/SubclassOf.h"
#include "Engine/DataTable.h"
#include "GenericStructs.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FPawnDataTable : public FTableRowBase {

	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPawnType PawnType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<APawn> PawnClass;
};

class ASSIGNMENT_1_2_API GenericStructs
{

public:
	GenericStructs();
	~GenericStructs();
};
