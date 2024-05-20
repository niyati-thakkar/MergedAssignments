// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericEnums.generated.h"

/**
 * 
 */

UENUM()
enum class EPawnType : uint8 {
	TopDown UMETA(DisplayName = "Top Down"),
	FirstPerson UMETA(DisplayName = "First Person"),
	ThirdPerson UMETA(DisplayName = "Third Person")
};

class ASSIGNMENT_1_2_API GenericEnums
{

public:
	GenericEnums();
	~GenericEnums();
};
