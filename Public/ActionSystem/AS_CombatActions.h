// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AS_CombatActions.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAS_CombatActions : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GAMEANIMATIONSAMPLEBASIC_API IAS_CombatActions
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Actions")
	void StartCombatAction();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Actions")
	void StopCombatAction();
};
