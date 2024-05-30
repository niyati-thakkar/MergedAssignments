#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/Actor.h"
#include "CoreMinimal.h"
#include "FenceDataAsset.generated.h"


USTRUCT(BlueprintType)
struct FFenceClassPair
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fence Data")
    TSubclassOf<AActor> ActorClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fence Data")
    UStaticMesh* MeshClass;
};
UENUM(BlueprintType)
enum class EFenceType : uint8
{
    RoundedOverTopCapital UMETA(DisplayName = "Rounded Over Top Capital"),
    ACornCapital UMETA(DisplayName = "ACorn Capital"),
    RoundTurnedCapital UMETA(DisplayName = "Round Turned Capital"),
    WindsorTurnedCapital UMETA(DisplayName = "Windsor Turned Capital"),
    GothicStarCapital UMETA(DisplayName = "Gothic Star Capital"),
    PyramidTop UMETA(DisplayName = "Pyramid Top"),
    Random UMETA(DisplayName = "Random")
};

UENUM(BlueprintType)
enum class EMaterialType : uint8
{
    Oak_Wood UMETA(DisplayName = "Oak Wood"),
    Pine_Wood UMETA(DisplayName = "Pine Wood"),
    Walnut_Wood UMETA(DisplayName = "Walnut Wood"),
    Walnut_Floor_Wood UMETA(DisplayName = "Walnut Floor Wood"),
    Random UMETA(DisplayName = "Random")
};


UCLASS(BlueprintType)
class ASSIGNMENT_5_6_API UFenceDataAsset : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fence Data")
    TMap<EFenceType, FFenceClassPair> FenceMap;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fence Data")
    TMap<EMaterialType, UMaterialInstance*> Materials;
};
