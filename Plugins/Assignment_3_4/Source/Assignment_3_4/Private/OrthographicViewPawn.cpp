// Fill out your copyright notice in the Description page of Project Settings.


#include "OrthographicViewPawn.h"




// Sets default values
AOrthographicViewPawn::AOrthographicViewPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
void AOrthographicViewPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOrthographicViewPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AOrthographicViewPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		UInputMappingContext* IMC = NewObject<UInputMappingContext>();

		// Look Action
		UInputAction* LookAction = NewObject<UInputAction>();
		LookAction->ValueType = EInputActionValueType::Axis2D;

		UInputModifierNegate* LookActionMouseNegateModifier = NewObject<UInputModifierNegate>();
		LookActionMouseNegateModifier->bX = false;
		LookActionMouseNegateModifier->bY = true;
		LookActionMouseNegateModifier->bZ = false;

		FEnhancedActionKeyMapping& LookActionMappedContext = IMC->MapKey(LookAction, EKeys::Mouse2D);
		LookActionMappedContext.Modifiers.Add(LookActionMouseNegateModifier);


		// Move Action 
		UInputAction* MoveAction = NewObject<UInputAction>();
		MoveAction->ValueType = EInputActionValueType::Axis3D;

		UInputModifierNegate* MoveActionNegateModifier = NewObject<UInputModifierNegate>();
		MoveActionNegateModifier->bX = true;
		MoveActionNegateModifier->bY = true;
		MoveActionNegateModifier->bZ = true;

		UInputModifierSwizzleAxis* SwizzleModifierDKey = NewObject<UInputModifierSwizzleAxis>();
		SwizzleModifierDKey->Order = EInputAxisSwizzle::YXZ;

		// Zoom Action 
		UInputAction* ZoomAction = NewObject<UInputAction>();
		ZoomAction->ValueType = EInputActionValueType::Axis3D;

		UInputModifierNegate* ZoomActionNegateModifier = NewObject<UInputModifierNegate>();
		MoveActionNegateModifier->bX = true;
		MoveActionNegateModifier->bY = true;
		MoveActionNegateModifier->bZ = true;

		// Mapping Keys
		IMC->MapKey(MoveAction, EKeys::W);

		FEnhancedActionKeyMapping& MoveForwardInputMapping = IMC->MapKey(MoveAction, EKeys::S);
		MoveForwardInputMapping.Modifiers.Add(MoveActionNegateModifier);

		FEnhancedActionKeyMapping& MoveRightInputMapping = IMC->MapKey(MoveAction, EKeys::D);
		MoveRightInputMapping.Modifiers.Add(SwizzleModifierDKey);

		FEnhancedActionKeyMapping& MoveLeftInputMapping = IMC->MapKey(MoveAction, EKeys::A);
		MoveLeftInputMapping.Modifiers.Add(SwizzleModifierDKey);
		MoveLeftInputMapping.Modifiers.Add(MoveActionNegateModifier);

		IMC->MapKey(MoveAction, EKeys::MouseScrollDown);

		FEnhancedActionKeyMapping& MouseUpInputMapping = IMC->MapKey(MoveAction, EKeys::MouseScrollUp);
		MouseUpInputMapping.Modifiers.Add(ZoomActionNegateModifier);

		// Bindings
		EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &AOrthographicViewPawn::Look);
		EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AOrthographicViewPawn::Move);
		EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AOrthographicViewPawn::HandleZoom);

		if (const APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
			if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
				SubSystem->ClearAllMappings();
				SubSystem->AddMappingContext(IMC, 0);
			}
		}


	}
}

void AOrthographicViewPawn::HandleZoom(const FInputActionValue& Value)
{

	float ZoomValue = Value.Get<float>();
	const float ZoomSpeed = 1000.0f; // Adjust zoom speed as needed
	float NewArmLength = FMath::Clamp(SpringArmComponent->TargetArmLength + ZoomValue * ZoomSpeed * GetWorld()->GetDeltaSeconds(), MinZoomLength, MaxZoomLength);

	SpringArmComponent->TargetArmLength = NewArmLength;
}
void AOrthographicViewPawn::Move(const FInputActionValue& Value)
{
	if (Value.GetMagnitude() == 0.0f) return;

	FVector2D MovementVector = Value.Get<FVector2D>();

	// Determine direction and magnitude
	FVector Direction = FVector(MovementVector.X, MovementVector.Y, 0.0f);
	if (Controller)
	{
		// Move pawn in the direction
		AddMovementInput(Direction, 1.0f, false);
	}
}

void AOrthographicViewPawn::Look(const FInputActionValue& Value)
{

	FVector2D LookInput = Value.Get<FVector2D>();

	if (Controller)
	{
		// Apply rotation based on look input
		AddControllerYawInput(LookInput.X);
		AddControllerPitchInput(LookInput.Y);
	}
}