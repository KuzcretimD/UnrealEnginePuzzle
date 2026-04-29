// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ActionSystem/AS_CombatActions.h"

#include "SandboxCharacter_CMC.generated.h"

UCLASS()
class GAMEANIMATIONSAMPLEBASIC_API ASandboxCharacter_CMC : public ACharacter, public IAS_CombatActions
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASandboxCharacter_CMC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// IAS_CombatActions native events implementation

	void StartCombatAction_Implementation() override;
	void StopCombatAction_Implementation() override;
};
