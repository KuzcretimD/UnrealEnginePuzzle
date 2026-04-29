// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AS_ActionTypes.h"
#include "GameplayTagContainer.h"
#include "AS_ActionSystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActionStart, FGameplayTag, ActionTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActionStop, FGameplayTag, ActionTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActiveTagsUpdated, FGameplayTagContainer, ActiveGameplayTags);

class UAS_Action;
class UAnimMontage;
class AActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEANIMATIONSAMPLEBASIC_API UAS_ActionSystemComponent : public UActorComponent
{
	GENERATED_BODY()
protected:
	// Called when the game starts
	virtual void BeginPlay() override;


	void InitializeActionContextData();

public:

	// Sets default values for this component's properties
	UAS_ActionSystemComponent();
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnActionStart OnActionStart;
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnActionStop OnActionStop;
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnActiveTagsUpdated OnActveTagsUpdated;



	//Used for tracking the active gameplay tags of the owner 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tags")
	FGameplayTagContainer ActiveGameplayTags;

	//Used for tracking the active gameplay tags of the owner 
	//Tells what actions are avaliable to take for example a weapon or other system migh add actions to this array. what can it do. I should think of a better name
	UPROPERTY()
	TArray<TObjectPtr<UAS_Action>> AvaliableActions;

	//Tells what action are currently activated for example player is atacking right now. what is it doing
	UPROPERTY()
	TArray<TObjectPtr<UAS_Action>> ActiveActions;

	UPROPERTY()
	TObjectPtr<UAS_Action> CurrentAction;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action System")
	TArray<TSubclassOf<UAS_Action>> DefaultActions;

	
	UFUNCTION(BlueprintCallable, Category = "Action System")
	void GrantActiveTags(const FGameplayTagContainer& TagsToAdd);

	UFUNCTION(BlueprintCallable, Category = "Action System")
	void RemoveActiveTags(const FGameplayTagContainer TagsToRemove);

	//  TODO: czy to powinno byæ blueprint callable ?
	UFUNCTION(BlueprintCallable, Category = "Action System")
	void AddAction(TSubclassOf<UAS_Action> ActionClass);
	UFUNCTION(BlueprintCallable, Category = "Action System")
	void AddActions(TArray<TSubclassOf<UAS_Action>> ActionsClasses);


	UFUNCTION(BlueprintCallable, Category = "Action System")
	void RemoveAction(TSubclassOf<UAS_Action> ActionClass);
	UFUNCTION(BlueprintCallable, Category = "Action System")
	void RemoveActions(TArray<TSubclassOf<UAS_Action>> ActionsClasses);
	

	UFUNCTION(BlueprintCallable, Category = "Action System")
	void SetCurrentAction(UAS_Action* Action) { CurrentAction = Action; }
	UFUNCTION(BlueprintCallable, Category = "Action System")
	UAS_Action* GetCurrentAction() { return CurrentAction; }

	UFUNCTION(BlueprintCallable, Category = "Action System")
	void RemoveCurrentAction() { CurrentAction = nullptr; }


	
	UFUNCTION(BlueprintCallable, Category = "Action System")
	void AddActiveAction(UAS_Action* NewAction);
	UFUNCTION(BlueprintCallable, Category = "Action System")
	void RemoveActiveAction(UAS_Action* ActionToRemove);
	UFUNCTION(BlueprintCallable, Category = "Action System")
	bool IsActionRunning(const FGameplayTag ActionToCheck) const;
	UFUNCTION(BlueprintCallable, Category = "Action System")
	

	bool StartActionByTag(FGameplayTag ActionToStart);
	UFUNCTION(BlueprintCallable, Category = "Action System")
	bool StopActionByTag(FGameplayTag ActionToStop);

	UFUNCTION(BlueprintCallable, Category = "Action System")
	bool StartActionByClass(const TSubclassOf<UAS_Action> ActionClass);
	UFUNCTION(BlueprintCallable, Category = "Action System")
	bool StopActionByClass(const TSubclassOf<UAS_Action> ActionClass);
	UFUNCTION(BlueprintCallable, Category = "Action System")
	bool StopCurrentAction();

	UFUNCTION(BlueprintCallable, Category = "Action System")
	float PlayActionMontage(UAnimMontage* MontageToPlay, const float PlayRate = 1.f, const FName StartSection = FName("none"));



	UFUNCTION(BlueprintCallable, Category = "Action System")
	void SetContextObject(UObject* ContextObject) { ActionContext.ContextObject = ContextObject; }
	void SetAvatarActor(AActor* AvatarActor) { ActionContext.AvatarActor = AvatarActor; }
	void SetAvatarCharater(ACharacter* AvatarCharater) { ActionContext.AvatarCharater = AvatarCharater; }
	void SetAvatarMesh(USkeletalMeshComponent* AvatarMesh) { ActionContext.AvatarMesh = AvatarMesh; }
	void SetAvatarAnimInstance(UAnimInstance* AnimInstance) { ActionContext.AvatarAnimInstance = AnimInstance; }


private:
	UPROPERTY()
	FAS_ActionContext ActionContext;
};