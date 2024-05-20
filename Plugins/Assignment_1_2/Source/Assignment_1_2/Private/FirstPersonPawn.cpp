// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstPersonPawn.h"

// Sets default values
AFirstPersonPawn::AFirstPersonPawn()
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

	static ConstructorHelpers::FObjectFinder<UPawnAttributeAsset> PawnAsset(TEXT("/Assignment_1_2/PawnSpawn/FirstPersonPawnAsset.FirstPersonPawnAsset"));
	PawnAttributes = PawnAsset.Object;
}

// Called when the game starts or when spawned
void AFirstPersonPawn::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AFirstPersonPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFirstPersonPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		UInputMappingContext* InputMappingContext = NewObject<UInputMappingContext>();

		// Look Action
		UInputAction* LookAction = NewObject<UInputAction>();
		LookAction->ValueType = EInputActionValueType::Axis2D;

		UInputModifierNegate* LookActionMouseNegateModifier = NewObject<UInputModifierNegate>();
		LookActionMouseNegateModifier->bX = false;
		LookActionMouseNegateModifier->bY = true;
		LookActionMouseNegateModifier->bZ = false;

		FEnhancedActionKeyMapping& LookActionMappedContext = InputMappingContext->MapKey(LookAction, EKeys::Mouse2D);
		LookActionMappedContext.Modifiers.Add(LookActionMouseNegateModifier);

		UInputAction* MoveAction = NewObject<UInputAction>();
		MoveAction->ValueType = EInputActionValueType::Axis3D;


		// Move Action Modifiers
		UInputModifierNegate* MoveActionNegateModifier = NewObject<UInputModifierNegate>();
		MoveActionNegateModifier->bX = true;
		MoveActionNegateModifier->bY = true;
		MoveActionNegateModifier->bZ = true;

		UInputModifierSwizzleAxis* SwizzleModifierDKey = NewObject<UInputModifierSwizzleAxis>();
		SwizzleModifierDKey->Order = EInputAxisSwizzle::YXZ;

		UInputModifierSwizzleAxis* SwizzleModifierEKey = NewObject<UInputModifierSwizzleAxis>();
		SwizzleModifierEKey->Order = EInputAxisSwizzle::ZYX;

		// Mapping Keys

		InputMappingContext->MapKey(MoveAction, EKeys::W);

		FEnhancedActionKeyMapping& MoveForwardInputmapping = InputMappingContext->MapKey(MoveAction, EKeys::S);
		MoveForwardInputmapping.Modifiers.Add(MoveActionNegateModifier);



		FEnhancedActionKeyMapping& MoveRightInputMapping = InputMappingContext->MapKey(MoveAction, EKeys::D);
		MoveRightInputMapping.Modifiers.Add(SwizzleModifierDKey);

		FEnhancedActionKeyMapping& MoveLeftInputMapping = InputMappingContext->MapKey(MoveAction, EKeys::A);
		MoveLeftInputMapping.Modifiers.Add(SwizzleModifierDKey);
		MoveLeftInputMapping.Modifiers.Add(MoveActionNegateModifier);



		FEnhancedActionKeyMapping& MoveUpInputMapping = InputMappingContext->MapKey(MoveAction, EKeys::E);
		MoveUpInputMapping.Modifiers.Add(SwizzleModifierEKey);

		FEnhancedActionKeyMapping& MoveDownInputMapping = InputMappingContext->MapKey(MoveAction, EKeys::Q);
		MoveDownInputMapping.Modifiers.Add(SwizzleModifierEKey);
		MoveDownInputMapping.Modifiers.Add(MoveActionNegateModifier);

		// Bindings
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFirstPersonPawn::Look);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFirstPersonPawn::Move);


		if (const APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
			if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
				//SubSystem->ClearAllMappings();
				SubSystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}

}

void AFirstPersonPawn::Look(const FInputActionValue& LookAction)
{
	FVector2D LookActionVector = LookAction.Get<FVector2D>();
	AddControllerYawInput(LookActionVector.X);
	AddControllerPitchInput(LookActionVector.Y);
}

void AFirstPersonPawn::Move(const FInputActionValue& MoveAction)
{
	FVector MovementVector = MoveAction.Get<FVector>();
	//GEngine->AddOnScreenDebugMessage(-1 ,1 ,  FColor::Cyan , "This ");
	FRotator Rotation = Controller->GetControlRotation();

	FVector ForwardDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);
	FVector RightDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);
	FVector UpDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Z);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
	AddMovementInput(UpDirection, MovementVector.Z);
	//FVector tmp = GetActorRotation().RotateVector(MovementVector);
	//AddMovementInput(tmp, 1.f);

}

