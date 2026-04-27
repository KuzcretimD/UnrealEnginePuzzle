// Fill out your copyright notice in the Description page of Project Settings.


#include "Puzzles/InteractionComponentPuzzle.h"
#include "Puzzles/PuzzleWall.h"
#include "AnimInstanceSandboxCharacter_CMC.h"

#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
// Sets default values for this component's properties
UInteractionComponentPuzzle::UInteractionComponentPuzzle()
{

	PrimaryComponentTick.bCanEverTick = false;

}




// Called when the game starts
void UInteractionComponentPuzzle::BeginPlay()
{
	Super::BeginPlay();
	PlayerCharacter = Cast<ACharacter>(GetOwner());
	if (PlayerCharacter) {
		PlayerControler = Cast<APlayerController>(PlayerCharacter->GetController());
		if (PlayerControler) {
			UE_LOG(LogTemp, Warning, TEXT("setup ok"));
		}
	}
	BindPuzzleInput();
}
void UInteractionComponentPuzzle::SetCurrentPuzzle(APuzzleWall* Puzzle)
{
	// nie jestem pewny robi� ten check czy od razu ustaia� curent puzzle

	UE_LOG(LogTemp, Display, TEXT("puzel dodany jako obecny puzel"));
		CurrentPuzzle = Puzzle;
	
}

void UInteractionComponentPuzzle::ClearCurrentPuzzle(APuzzleWall* PuzzleWall)
{
	//mo�na to zrobi� inaczej narazie jest tak
	if (CurrentPuzzle == PuzzleWall && !bIsInteracting) {
		CurrentPuzzle = nullptr;
		UE_LOG(LogTemp, Display, TEXT("puzel ununiety jako obecny puzel"));
	}
}
void UInteractionComponentPuzzle::BindPuzzleInput()
{
	if (!PlayerControler) { return; }
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerControler->InputComponent)) {
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &UInteractionComponentPuzzle::OnInteractPressed);
	}
}
void UInteractionComponentPuzzle::OnInteractPressed(const FInputActionValue& Value){
	const bool pressed = Value.Get<bool>();
	//nie jestem jescze penwy jak działą trigger więc sprawdze też wrazie czego czy nie dostaje tutaj false z value;
	if (!pressed) { return; }
	if (Value.Get<bool>() && !bIsInteracting) {
		EnterPuzzle();
	}
	else {
		ExitPuzzle();
	}

}
void UInteractionComponentPuzzle::EnterPuzzle()
{
	if (!PlayerCharacter || !PlayerControler || !CurrentPuzzle) { return; }
	bIsInteracting = true;
	PreviouseViewTarget = PlayerControler->GetViewTarget();
	CurrentPuzzle->StartPuzzle(PlayerCharacter,PlayerControler);
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerControler->GetLocalPlayer())) {
		if (PuzzleMappingContext) {
			UE_LOG(LogTemp, Display, TEXT("Dodano mapping"));
			Subsystem->AddMappingContext(PuzzleMappingContext,100);
			if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerControler->InputComponent)) {
				//nie wiem czy trzeba to sprawdzać wydaje się mi się że tak ale no muszę się dowiedzieć 
				if (MouseMoveAction) {
					EnhancedInputComponent->BindAction(MouseMoveAction,ETriggerEvent::Triggered, this, &UInteractionComponentPuzzle::OnMouseMove);
					UE_LOG(LogTemp, Display, TEXT("OnMouseMove zbindowane"));
				}
				if(LeftClickAction) {
					EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Triggered, this, &UInteractionComponentPuzzle::OnLeftClick);
				}
			}
		}
	}


}
void UInteractionComponentPuzzle::ExitPuzzle()
{
	if (!PlayerControler || !CurrentPuzzle || !bIsInteracting) { return; }

	CurrentPuzzle->EndPuzzle();
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerControler->GetLocalPlayer())) {
		if (PuzzleMappingContext) {
			Subsystem->RemoveMappingContext(PuzzleMappingContext);
		}
	}
	if (PreviouseViewTarget){
		PlayerControler->SetViewTargetWithBlend(PreviouseViewTarget, 0.25);
	}

	PlayerCharacter->EnableInput(PlayerControler);
	if (TObjectPtr<UAnimInstanceSandboxCharacter_CMC> PlayerAnimInstance = Cast<UAnimInstanceSandboxCharacter_CMC>(PlayerCharacter->GetMesh()->GetAnimInstance())) {
		PlayerAnimInstance->SetPuzzleMode(false, FVector().ZeroVector, FRotator().ZeroRotator);
	}
	bIsInteracting = false;
}
void UInteractionComponentPuzzle::OnMouseMove(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Display, TEXT("próba ruchu ręki"));
	if (bIsInteracting && CurrentPuzzle) {
		UE_LOG(LogTemp, Display, TEXT("próba ruchu ręki 2"));
		CurrentPuzzle->MoveHand(Value.Get<FVector2D>(),PlayerCharacter);
	}
}

void UInteractionComponentPuzzle::OnLeftClick(const FInputActionValue& Value)
{
	if (bIsInteracting && CurrentPuzzle) {
		CurrentPuzzle->HandleGrab(Value.Get<bool>());
	}
}








