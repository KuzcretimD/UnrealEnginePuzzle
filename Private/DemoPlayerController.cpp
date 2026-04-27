// Fill out your copyright notice in the Description page of Project Settings.


#include "DemoPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
void ADemoPlayerController::SetupInputComponent() {
	if (IsLocalPlayerController())
	{
		//Add Input Mapping Context
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
		return;
	}
}