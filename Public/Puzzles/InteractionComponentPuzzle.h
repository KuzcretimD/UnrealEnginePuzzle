// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponentPuzzle.generated.h"

class APuzzleWall;
class UInputAction;
class UInputMappingContext;
class ACharacter;
class APlayerController;
struct FInputActionValue;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEANIMATIONSAMPLEBASIC_API UInteractionComponentPuzzle : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractionComponentPuzzle();

	void SetCurrentPuzzle(APuzzleWall *PuzzleWall);
	void ClearCurrentPuzzle(APuzzleWall *PuzzleWall);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle | Input")
	TObjectPtr<UInputMappingContext> PuzzleMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle | Input")
	TObjectPtr<UInputAction> InteractAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle | Input")
	TObjectPtr<UInputAction> MouseMoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle | Input")
	TObjectPtr<UInputAction> LeftClickAction;

	private : 

	UPROPERTY()
	TObjectPtr<APuzzleWall> CurrentPuzzle;
	UPROPERTY()
	TObjectPtr<ACharacter> PlayerCharacter;
	UPROPERTY()
	TObjectPtr<APlayerController> PlayerControler;
	UPROPERTY()
	TObjectPtr<AActor> PreviouseViewTarget;

	bool bIsInteracting = false;

	UFUNCTION()
	void OnInteractPressed(const FInputActionValue &Value);
	UFUNCTION()
	void OnMouseMove(const FInputActionValue &Value);
	UFUNCTION()
	void OnLeftClick(const FInputActionValue &Value);
	UFUNCTION()
	void EnterPuzzle();
	UFUNCTION()
	void ExitPuzzle();
	UFUNCTION()
	void BindPuzzleInput();

		
};
