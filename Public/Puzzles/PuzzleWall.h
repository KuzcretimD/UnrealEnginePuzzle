// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzleWall.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class UCameraComponent;
class UInputMappingContext;
class ACharacter;
class APlayerController;
class UInputAction;
class USceneComponent;
class UUserWidget;
struct FInputActionValue;


UCLASS()
class GAMEANIMATIONSAMPLEBASIC_API APuzzleWall : public AActor
{
	GENERATED_BODY()
	
public:	

	APuzzleWall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//this is default component nessesary for logic this should be a wall that tells the hand what the movement should be add your own and add appropriate tag ->  
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Puzzle | Component")
	TObjectPtr<UStaticMeshComponent> WallMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Puzzle | Components")
	UStaticMeshComponent* InteractionPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Puzzle | Components")
	UBoxComponent* InteractionZone;

	//Wiem ¿e to s³aby pomys³ w ten sposób robiæ UI i lepiej by³o by mieæ jeden centralny punkt i dodawaæ do niego poprostu dane ale jest póŸno bardzo chêtnie to zrobie póŸniej 
	//TODO Find a better way to make ui in Unreal 

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Puzzle | UI")
	TSubclassOf<UUserWidget> PuzzleStartUI;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Puzzle | UI")
	TSubclassOf<UUserWidget> PuzzleExitUI;

	UPROPERTY()
	TObjectPtr<UUserWidget> PuzzleUIInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Puzzle | Components")
	UCameraComponent* PuzzleCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Puzzle | PlayerComponents")
	UCameraComponent* PlayerCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Puzzle | PlayerComponents")
	TObjectPtr<ACharacter> PlayerCharater;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Puzzle | PlayerComponents")
	TObjectPtr<APlayerController> PlayerControler;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Puzzle | PlayerComponents")
	FVector WorldHandTargetLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	//Ca³y ten system raczej nie powinien byæ w tym pliku 
	//TODO Change how the whole Input system is setup 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> MouseMove;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> LeftClick;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> Interact;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle | Settings")
	float MouseSensitivity = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle | Settings")
	float GrabRadius = 20.f; // Jak blisko musi byæ rêka (w cm), ¿eby da³o siê z³apaæ punkt

	//FLAGI
	bool bIsInteracting = false;
	bool bIsPuzzleSetupValid = false;
	bool bTryGrabing = false;
	bool bTryingToInteract = true;
	//FUNKCJE



	UFUNCTION(BlueprintCallable, Category = "Puzzle | Functionality")
	void StartPuzzle();
	//	void StartPuzzle(APlayerController* PlayerControler, ACharacter* PlayerCharater);
	UFUNCTION(BlueprintCallable, Category = "Puzzle | Functionality")
	void EndPuzzle();

	UFUNCTION(BlueprintCallable, Category = "Puzzle | UI")
	void SuggestInteraction();

	UFUNCTION(BlueprintCallable, Category = "Puzzle | UI")
	void SuggestInteractionEnd();
	UFUNCTION(BlueprintCallable, Category = "Puzzle | UI")
	void ClearPuzzleUI();

	UFUNCTION(BlueprintCallable, Category = "Puzzle | Functionality")
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void MoveHand(const FInputActionValue &Value);
	UFUNCTION()
	void HandleGrab(const FInputActionValue& Value);
	void SetTryingToInteract(const FInputActionValue& Value);
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

};
