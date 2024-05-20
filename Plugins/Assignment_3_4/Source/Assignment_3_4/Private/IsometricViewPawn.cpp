// Fill out your copyright notice in the Description page of Project Settings.


#include "IsometricViewPawn.h"



// Sets default values
AIsometricViewPawn::AIsometricViewPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    bHasHitLocation = false;

    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
    CapsuleComponent->SetCapsuleHalfHeight(90);
    CapsuleComponent->SetCapsuleRadius(30);
    RootComponent = CapsuleComponent;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    SpringArmComponent->TargetArmLength = 20;
    SpringArmComponent->SetupAttachment(RootComponent);

    SpringArmComponent->bUsePawnControlRotation = true;

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);

    FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));

}

// Called when the game starts or when spawned
void AIsometricViewPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AIsometricViewPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AIsometricViewPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
        UInputMappingContext* IMC = NewObject<UInputMappingContext>();

        UInputAction* ClockWiseRotation = NewObject<UInputAction>();
        ClockWiseRotation->ValueType = EInputActionValueType::Axis2D;
        UInputAction* AntiClockWiseRotation = NewObject<UInputAction>();
        AntiClockWiseRotation->ValueType = EInputActionValueType::Axis2D;


        IMC->MapKey(ClockWiseRotation, EKeys::E);
        IMC->MapKey(AntiClockWiseRotation, EKeys::Q);


        EIC->BindAction(ClockWiseRotation, ETriggerEvent::Completed, this, &AIsometricViewPawn::ClockwiseRotation);
        EIC->BindAction(AntiClockWiseRotation, ETriggerEvent::Completed, this, &AIsometricViewPawn::AntiClockwiseRotation);

        if (const APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
            if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
                SubSystem->ClearAllMappings();
            	SubSystem->AddMappingContext(IMC, 0);
            }
        }
        IsometricView();
    }
}



void AIsometricViewPawn::IsometricView()
{

    SpringArmComponent->bInheritPitch = false;
    SpringArmComponent->bInheritYaw = false;
    SpringArmComponent->bInheritRoll = false;
    SpringArmComponent->TargetArmLength = 500.0f; // Adjust arm length as needed

    FRotator TargetRotation;
    FVector TargetOffset;

    float AngleIncrement = 45.0f; // Angle increment between each position
    float Radius = 50.0f; // Radius of the circle

    // Calculate the target offset based on the current OGVPosition
    switch (IMPosition)
    {
    case 0:
        TargetOffset = FVector(Radius, 0.0f, 50.0f);
        TargetRotation = FRotator(-35.26f, 0.0f, 0.0f);
        break;
    case 1:
        TargetOffset = FVector(Radius * FMath::Cos(FMath::DegreesToRadians(AngleIncrement)), Radius * FMath::Sin(FMath::DegreesToRadians(AngleIncrement)), 50.0f);
        TargetRotation = FRotator(-35.26f, 45.0f, 0.0f);
        break;
    case 2:
        TargetOffset = FVector(0.0f, Radius, 50.0f);
        TargetRotation = FRotator(-35.26f, 90.0f, 0.0f);
        break;
    case 3:
        TargetOffset = FVector(Radius * FMath::Cos(FMath::DegreesToRadians(AngleIncrement * 3)), Radius * FMath::Sin(FMath::DegreesToRadians(AngleIncrement * 3)), 50.0f);
        TargetRotation = FRotator(-35.26f, 135.0f, 0.0f);
        break;
    case 4:
        TargetOffset = FVector(-Radius, 0.0f, 50.0f);
        TargetRotation = FRotator(-35.26f, 180.0f, 0.0f);
        break;
    case 5:
        TargetOffset = FVector(Radius * FMath::Cos(FMath::DegreesToRadians(AngleIncrement * 5)), -Radius * FMath::Sin(FMath::DegreesToRadians(AngleIncrement * 5)), 50.0f);
        TargetRotation = FRotator(-35.26f, 225.0f, 0.0f);
        break;
    case 6:
        TargetOffset = FVector(0.0f, -Radius, 50.0f);
        TargetRotation = FRotator(-35.26f, 270.0f, 0.0f);
        break;
    case 7:
        TargetOffset = FVector(Radius * FMath::Cos(FMath::DegreesToRadians(AngleIncrement * 7)), -Radius * FMath::Sin(FMath::DegreesToRadians(AngleIncrement * 7)), 50.0f);
        TargetRotation = FRotator(-35.26f, 315.0f, 0.0f);
        break;
    }

    SpringArmComponent->TargetOffset = TargetOffset;
    SpringArmComponent->SetRelativeRotation(TargetRotation);
}


void AIsometricViewPawn::ClockwiseRotation(const FInputActionValue& MoveAction)
{

    IMPosition = (IMPosition + 1) % 8;
    IsometricView();
}

void AIsometricViewPawn::AntiClockwiseRotation(const FInputActionValue& MoveAction)
{

    IMPosition = (IMPosition + 7) % 8;
    IsometricView();
}

