// Fill out your copyright notice in the Description page of Project Settings.


#include "StaircaseActor.h"

// Sets default values
AStaircaseActor::AStaircaseActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    SceneComponentStairs = CreateDefaultSubobject<USceneComponent>("USceneForStairs");

    StairsLength = 1.0f;
    StairsWidth = 2.0f;
    StairsDepth = 0.30f;
    StepDepth = 100;
    // Define variables for number of stairs and spacing
    NumberOfStairs = 10;
    StairSpacing = 75.0f;  // Distance between stairs in X

    // Define variables for railing and handle dimensions
    RailingWidth = 0.75f;
    RailingThickness = 0.02f;
    RailingDepth = 5.0f;

    HandleLength = 1.0f;  // Adjust size of wood cubes as needed
    HandleWidth = 1.0f;  // Height of the wood cube on top of the railing
    HandleDepth = 0.05f;

    RailingLengthOffset = -10.0f;  // Half width for left railing placement
    RailingWidthOffset = 45.0f;  // Half width for right railing placement
    RailingDepthOffset = 100.0f;  // Adjust for positioning on top of railing
   
    HandleLengthOffset = 1.0f;  // Half width for left railing placement
    HandleWidthOffset = 1.0f;  // Half width for right railing placement
    HandleDepthOffset = 100.0f;
    
    SourceMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube")).Object;
    StairsMaterial = ConstructorHelpers::FObjectFinder<UMaterialInterface>(TEXT("/Game/StarterContent/Materials/M_Brick_Clay_New.M_Brick_Clay_New")).Object;

    GlassMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Game/StarterContent/Props/SM_GlassWindow.SM_GlassWindow")).Object;
    GlassMaterial = ConstructorHelpers::FObjectFinder<UMaterialInterface>(TEXT("/Game/StarterContent/Materials/M_Glass.M_Glass")).Object; // Material for glass railing

    HandleMaterial = ConstructorHelpers::FObjectFinder<UMaterialInterface>(TEXT("/Game/StarterContent/Materials/M_Wood_Floor_Walnut_Polished.M_Wood_Floor_Walnut_Polished")).Object; // Material for glass railing

    StairType = EStairType::BoxStairs;
}

// Called when the game starts or when spawned
void AStaircaseActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStaircaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AStaircaseActor::OnConstruction(const FTransform& transform) {
    Super::OnConstruction(transform);

    for (int i = OutMeshes.Num() - 1; i >= 0; i--) {
        FSubStair* SubStair = OutMeshes[i];

        // Check if the element is valid before calling DestroyStaircaseComponents
        if (SubStair != nullptr) {
            DestoryStaircaseComponents(SubStair); // Call the destroy function
        }
    }

    OutMeshes.Empty();
    // Call the function to create staircase components on construction
    for (int i = 0; i < NumberOfStairs; i++) {
        CreateStaircaseComponent(i);
    }
    
}

void AStaircaseActor::CreateStaircaseComponent(int Index) {

    

    FSubStair* SubStair = new FSubStair;
    RailingDepth = 4.0f;
    float StairsDepthScale = StairsDepth;
    StepDepth = 100;
    if (StairType == EStairType::ClosedStairs) {
        StepDepth = 100;
    }
    if (StairType == EStairType::OpenStairs) {
        StepDepth = 110;
        StairsDepthScale = 0.20f;
        RailingDepth *= 1.5;
    }
    if (StairType == EStairType::BoxStairs) {
        StepDepth = 0;
        Index += 1;
        StairsDepthScale = StairsDepth * (Index);
        RailingDepth /= (Index);
    }
    
    
    
    

    // Create and configure the stair mesh component
    SubStair->StairComponent = NewObject<UStaticMeshComponent>(this);
    SubStair->StairComponent->SetMaterial(0, StairsMaterial);
    SubStair->StairComponent->AttachToComponent(SceneComponentStairs, FAttachmentTransformRules::KeepRelativeTransform);
    SubStair->StairComponent->SetRelativeScale3D(FVector{ StairsLength, StairsWidth, StairsDepthScale });
    SubStair->StairComponent->SetStaticMesh(SourceMesh);
    
    SubStair->StairComponent->RegisterComponentWithWorld(GetWorld());
    
    // Use calculated position for stair placement
    FVector RelativeLocation = { StairsLength * (Index) * StairSpacing, 0, (Index) * StairsDepth * StepDepth };
    SubStair->StairComponent->SetRelativeLocation(RelativeLocation);

    // Create and configure the railing components (left and right)
    SubStair->LeftGlassRailing = NewObject<UStaticMeshComponent>(this);
    SubStair->RightGlassRailing = NewObject<UStaticMeshComponent>(this);
    SubStair->LeftGlassRailing->AttachToComponent(SubStair->StairComponent, FAttachmentTransformRules::KeepRelativeTransform);
    SubStair->RightGlassRailing->AttachToComponent(SubStair->StairComponent, FAttachmentTransformRules::KeepRelativeTransform);
    SubStair->LeftGlassRailing->SetRelativeScale3D(FVector{ RailingWidth, RailingThickness, RailingDepth});
    SubStair->RightGlassRailing->SetRelativeScale3D(FVector{ RailingWidth, RailingThickness, RailingDepth });
    SubStair->LeftGlassRailing->SetStaticMesh(SourceMesh);
    SubStair->RightGlassRailing->SetStaticMesh(SourceMesh);
    SubStair->LeftGlassRailing->SetMaterial(0, GlassMaterial);
    SubStair->RightGlassRailing->SetMaterial(0, GlassMaterial);
    SubStair->LeftGlassRailing->RegisterComponentWithWorld(GetWorld());
    SubStair->RightGlassRailing->RegisterComponentWithWorld(GetWorld());
    SubStair->LeftGlassRailing->SetRelativeLocation(FVector{ RailingLengthOffset, -RailingWidthOffset, RailingDepthOffset });
    SubStair->RightGlassRailing->SetRelativeLocation(FVector{ RailingLengthOffset, RailingWidthOffset, RailingDepthOffset });


    // Create and configure the top handle components (left and right)
    SubStair->LeftGlassRailingTopWood = NewObject<UStaticMeshComponent>(this);
    SubStair->RightGlassRailingTopWood = NewObject<UStaticMeshComponent>(this);
    SubStair->LeftGlassRailingTopWood->AttachToComponent(SubStair->LeftGlassRailing, FAttachmentTransformRules::KeepRelativeTransform);
    SubStair->RightGlassRailingTopWood->AttachToComponent(SubStair->RightGlassRailing, FAttachmentTransformRules::KeepRelativeTransform);
    SubStair->LeftGlassRailingTopWood->SetRelativeScale3D(FVector{ HandleWidth, HandleWidth, HandleDepth });
    SubStair->RightGlassRailingTopWood->SetRelativeScale3D(FVector{ HandleWidth, HandleWidth, HandleDepth });  // Typo fixed
    SubStair->LeftGlassRailingTopWood->SetStaticMesh(SourceMesh);
    SubStair->RightGlassRailingTopWood->SetStaticMesh(SourceMesh);
    SubStair->LeftGlassRailingTopWood->SetMaterial(0, HandleMaterial);
    SubStair->RightGlassRailingTopWood->SetMaterial(0, HandleMaterial);
    SubStair->LeftGlassRailingTopWood->RegisterComponentWithWorld(GetWorld());
    SubStair->RightGlassRailingTopWood->RegisterComponentWithWorld(GetWorld());
    // Set relative location for top railing components
    SubStair->LeftGlassRailingTopWood->SetRelativeLocation(FVector{ HandleLengthOffset, HandleWidthOffset, HandleDepthOffset });
    SubStair->RightGlassRailingTopWood->SetRelativeLocation(FVector{ HandleLengthOffset, HandleWidthOffset, HandleDepthOffset });

    
    

    

    // Add the SubStair pointer to the Meshes array for potential future use or cleanup
    OutMeshes.Add(SubStair);
}

void AStaircaseActor::DestoryStaircaseComponents(FSubStair* obj){
	
	if (obj->RightGlassRailingTopWood) {
		obj->RightGlassRailingTopWood->DestroyComponent();
		obj->RightGlassRailingTopWood = nullptr;
	}

	if (obj->LeftGlassRailingTopWood) {
		obj->LeftGlassRailingTopWood->DestroyComponent();
		obj->LeftGlassRailingTopWood = nullptr;
	}

    if (obj->RightGlassRailing) {
        obj->RightGlassRailing->DestroyComponent();
        obj->RightGlassRailing = nullptr;
    }

    if (obj->LeftGlassRailing) {
        obj->LeftGlassRailing->DestroyComponent();
        obj->LeftGlassRailing = nullptr;
    }

    if (obj->StairComponent) {
        obj->StairComponent->DestroyComponent();
        obj->StairComponent = nullptr;
    }
}
