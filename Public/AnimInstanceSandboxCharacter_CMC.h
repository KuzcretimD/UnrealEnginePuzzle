// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimInstanceSandboxCharacter_CMC.generated.h"


/**
 * 
 */
UCLASS()
class GAMEANIMATIONSAMPLEBASIC_API UAnimInstanceSandboxCharacter_CMC : public UAnimInstance
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Puzzles | Wallpuzzle")
	float BlendHand = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Puzzles | Wallpuzzle")
	bool bInPuzzleMode = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Puzzles | Wallpuzzle")
	FVector HandTargetLocation = FVector(0,0,0);
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Puzzles | Wallpuzzle")
	FRotator WallRotation;
public:

	UFUNCTION(BlueprintCallable, Category = "Puzzles | Wallpuzzle")
	void SetPuzzleMode(bool inPuzzleMode, FVector TargetLocation, FRotator Rotation);

	UFUNCTION(BlueprintCallable, Category = "Puzzles | Wallpuzzle")
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;



};
