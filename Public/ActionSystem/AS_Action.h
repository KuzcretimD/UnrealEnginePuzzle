// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include <GameplayTagContainer.h>
#include "AS_ActionTypes.h"
#include "AS_Action.generated.h"

/**
 * 
 */

UCLASS(Blueprintable, meta = (DisplayName = "Action"))
class GAMEANIMATIONSAMPLEBASIC_API UAS_Action : public UObject
{
	GENERATED_BODY()
public:
	//Tag to find an action without a reference to the object 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Action")
	FGameplayTag ActionTag;

	// Tags added to Owner  when action starts and removed when action stops. 
	UPROPERTY(EditAnywhere, Category = "Action")
	FGameplayTagContainer GrantedTags;

	//Action can only start if actor has none of this tags/flags TODO: pobably should rename it to blocking tags because they are teling what actions/statuses blocks the action from hapening not what actions can't happend during this action it's conffusing sorry.. :{ 
	UPROPERTY(EditAnywhere, Category = "Action")
	FGameplayTagContainer BlockedTags;

	//Current Montage In Action :)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	FAS_ActionMontage ActionMontage;



	UFUNCTION(BlueprintCallable, Category = "Action")
	bool OverrideCurrentAction();
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(const FAS_ActionContext& ActionContext);
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StopAction(const FAS_ActionContext& ActionContext);
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStartAction(const FAS_ActionContext& ActionContext) const;
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStopAction(const FAS_ActionContext& ActionContext) const;
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	float PlayActionMotage(UAnimMontage* MontageToPlay, const float PlayRate, const FName StartSection);

	virtual bool SatisfyTargetRequirements(const FGameplayTagContainer& ActiveGameplayTags) const;



	UFUNCTION(BlueprintCallable, Category = "Action System")
	UAS_ActionSystemComponent* GetActionSystemComponent()const;
	
};
