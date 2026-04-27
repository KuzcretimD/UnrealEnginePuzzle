// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstanceSandboxCharacter_CMC.h"
#include "Math/UnrealMathUtility.h"
void UAnimInstanceSandboxCharacter_CMC::SetPuzzleMode(bool inPuzzleMode, FVector TargetLocation, FRotator Rotation)
{
	if (inPuzzleMode) {
		HandTargetLocation = TargetLocation;
		bInPuzzleMode = true;
		WallRotation = Rotation;
	}
	else {
		//prawdopodobnie nie potrzebnie ustaiam hand location na 0 i tak bêdzie zmieniony jak gracz wejdzie w innny puzel marnowanie zasobów 
		//narazie sprawdzam jak wszystko dzia³a wiêc zostawiam tak 
		HandTargetLocation = FVector(0, 0, 0);
		bInPuzzleMode = false;

	}
}

void UAnimInstanceSandboxCharacter_CMC::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	float BlendTo = bInPuzzleMode ? BlendTo = 1.f : BlendTo = 0.f;
	BlendHand = FMath::FInterpTo(BlendHand, BlendTo, DeltaSeconds, 1.f);
	
}