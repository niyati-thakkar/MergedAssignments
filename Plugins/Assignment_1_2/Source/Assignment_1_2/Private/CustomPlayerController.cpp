// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPlayerController.h"
#include "BaseCharacter.h"



ACustomPlayerController::ACustomPlayerController()
{
	PawnTypeDataTable = nullptr;
	static ConstructorHelpers::FObjectFinder<UDataTable> PawnTypeDT(TEXT("/Assignment_1_2/PawnSpawn/PawnData.PawnData"));
	if (PawnTypeDT.Succeeded())
	{
		const FString ContextString{ TEXT("Pawn Type") };
		PawnTypeDataTable = PawnTypeDT.Object;
		PawnTypeDataTable->GetAllRows<FPawnDataTable>(ContextString, TypesOfPawns);
		CurrentPawnLocation = FVector{ 0, 0, 100 };
		CurrentPawnRotation = FRotator{ 0 };
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("got Data table"));
	}
	Index = 0;
	Size = TypesOfPawns.Num();
	
}

void ACustomPlayerController::SetupInputComponent() {

	Super::SetupInputComponent(); 
	SetEnhancedInputToggle();
	SpawnPawn();
	

	
}

void ACustomPlayerController::SetEnhancedInputToggle() {
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent)) {
		UInputMappingContext* InputMappingContext = NewObject<UInputMappingContext>();
		UInputAction* SwapAction = NewObject<UInputAction>();
		FEnhancedActionKeyMapping& PKeyPressMappedContext = InputMappingContext->MapKey(SwapAction, EKeys::P);

		if (SwapAction) {
			EnhancedInputComponent->BindAction(SwapAction, ETriggerEvent::Completed, this, &ACustomPlayerController::SpawnPawn);
		}
		if (UEnhancedInputLocalPlayerSubsystem* LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {
			if (LocalPlayerSubsystem) {
				LocalPlayerSubsystem->ClearAllMappings();
				LocalPlayerSubsystem->AddMappingContext(InputMappingContext, 0);
			}

		}
	}
}

void ACustomPlayerController::SpawnPawn() {
	if (PawnTypeDataTable) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("got Data table"));

		FPawnDataTable* CurrentPawnType = TypesOfPawns[Index];
		if (CurrentPawnType) {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("got struct"));
			if (UWorld* CurrentWorld = GetWorld()) {
				/*if(CurrentPawnType->PawnType == EPawnType::FirstPerson)
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("1"));
				if (CurrentPawnType->PawnType == EPawnType::ThirdPerson)
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("2"));
				if (CurrentPawnType->PawnType == EPawnType::TopDown)
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("3"));*/
				if (CurrentPawn) {
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, TEXT("P Key Pressed! Spawning Another Pawn..."));
					CurrentPawnLocation = CurrentPawn->GetActorLocation();
					CurrentPawnRotation = CurrentPawn->GetActorRotation();
					CurrentPawn->Destroy();
					CurrentPawn = nullptr;
				}
				FActorSpawnParameters SpawnParameter;
				SpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
				APawn* tempPawn = CurrentWorld->SpawnActor<APawn>(CurrentPawnType->PawnClass, CurrentPawnLocation, CurrentPawnRotation, SpawnParameter);
				SetEnhancedInputToggle();
				if (tempPawn != nullptr) {
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, TEXT("Spawned Pawn"));
					
					Possess(tempPawn);
					
					if (tempPawn->GetController())
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, TEXT("Possessed Pawn"));
					CurrentPawn = tempPawn;
					if (CurrentPawnType->PawnType == EPawnType::TopDown) {
						bShowMouseCursor = true;
						/*bEnableClickEvents = true;
						bEnableTouchEvents = true;
						bEnableMouseOverEvents = true;
						DefaultMouseCursor = EMouseCursor::Default;
						DefaultClickTraceChannel = ECollisionChannel::ECC_Visibility;*/
						//DisplayAttributes(Cast<ABaseCharacter>(SpawnedPawn)->PawnAttributeAsset);
					}
					else {
						bShowMouseCursor = false;
						/*bEnableClickEvents = false;
						bEnableTouchEvents = false;
						bEnableMouseOverEvents = false;*/
						//DisplayAttributes(Cast<ABaseCharacter>(SpawnedPawn)->PawnAttributeAsset);
					}
					if (CurrentPawnType->PawnType == EPawnType::FirstPerson) {
						DisplayAttributes(Cast<AFirstPersonPawn>(CurrentPawn)->PawnAttributes);
					}
					else {
						DisplayAttributes(Cast<ABaseCharacter>(CurrentPawn)->PawnAttributes);
					}
				}
			}

		}
	}
	Toggle();
	
		
}

void ACustomPlayerController::Toggle() {
	Index = ((Index + 1) % Size);
}
void ACustomPlayerController::BeginPlay()
{
	//SpawnPawn();
}
