// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/AS_ActionSystemComponent.h"
#include "ActionSystem/AS_Action.h"
#include "GameFramework/Character.h"
// Sets default values for this component's properties
UAS_ActionSystemComponent::UAS_ActionSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UAS_ActionSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeActionContextData();

	AddActions(DefaultActions);

}

void UAS_ActionSystemComponent::InitializeActionContextData()
{
	if (AActor* AvatarActor = GetOwner()) {
		SetAvatarActor(AvatarActor);
	}
	if (ACharacter* AvatarCharacter = Cast<ACharacter>(ActionContext.AvatarActor)) {
		SetAvatarCharater(AvatarCharacter);
		SetAvatarMesh(AvatarCharacter->GetMesh());
		SetAvatarAnimInstance(ActionContext.AvatarMesh->GetAnimInstance());

	}
}

//Broadcastd the event which and gives you what was added to ActiveGameplayTags
void UAS_ActionSystemComponent::GrantActiveTags(const FGameplayTagContainer& TagsToAdd)
{
	ActiveGameplayTags.AppendTags(TagsToAdd);
	OnActveTagsUpdated.Broadcast(TagsToAdd);
}

//Broadcastd the event which gives you what was removed from ActiveGamplayTags 
void UAS_ActionSystemComponent::RemoveActiveTags(const FGameplayTagContainer TagsToRemove)
{
	if (!ActiveGameplayTags.IsEmpty()) {
		ActiveGameplayTags.RemoveTags(TagsToRemove);
		OnActveTagsUpdated.Broadcast(TagsToRemove);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Trying to remove tag from empty ActiveGameplayTags container"));
	}

}

void UAS_ActionSystemComponent::AddAction(TSubclassOf<UAS_Action> ActionClass)
{

	if (!ActionClass) {
		UE_LOG(LogTemp, Warning, TEXT("UAS_ActionSystemComponent::AddAction -> Could not add new Action to ActiveActions, Action not valid"));
		return;
	}
	UAS_Action* NewAction = NewObject<UAS_Action>(this, ActionClass);

	AvaliableActions.Add(NewAction);


}
void UAS_ActionSystemComponent::AddActions(TArray<TSubclassOf<UAS_Action>> ActionsClasses)
{
	for (TSubclassOf<UAS_Action> Action : ActionsClasses) {
		UE_LOG(LogTemp, Warning, TEXT("Trying to add actions"));
		AddAction(Action);
	}
}
void UAS_ActionSystemComponent::RemoveAction(TSubclassOf<UAS_Action> ActionClass)
{
	if (!ActionClass) {
		UE_LOG(LogTemp, Warning, TEXT("UAS_ActionSystemComponent::RemoveAction -> Could not remove Action from AvaliableActions, Action not valid"));
		return;
	}
	if (!AvaliableActions.IsEmpty()) {
		for (int i = AvaliableActions.Num() - 1; i >= 0; i--) {
			if (AvaliableActions[i] && AvaliableActions[i]->GetClass() == ActionClass) {
				UE_LOG(LogTemp, Warning, TEXT("REMOVED ACTION From AveliableActions"))
					//@DONE @OPTIMISE skoro mam ju� index elementu kt�ry chc� usun�� to sparwdz czy nie ma funkcji kt�ra to usunie 
					//bo zgaduj� �e �e Remove() b�dzie jescze raz loopow� przez ca�� tablicetablice
					//w tym wypadku to nie b�dzie mia�o du�ego wp�ywu na performance ale przyda si� to wwiedzie� na przysz�o��
					//AvaliableActions.Remove(AvaliableActions[i]);
					AvaliableActions.RemoveAt(i);
			}
		}
	}
}

void UAS_ActionSystemComponent::RemoveActions(TArray<TSubclassOf<UAS_Action>> ActionsClasses)
{
	for (TSubclassOf<UAS_Action> Action : ActionsClasses) {
		UE_LOG(LogTemp, Warning, TEXT("Trying to add default actions"));
		RemoveAction(Action);
	}
}

void UAS_ActionSystemComponent::AddActiveAction(UAS_Action* NewAction)
{
	if (!NewAction) {
		UE_LOG(LogTemp, Warning, TEXT("UAS_ActionSystemComponent::AddActiveAction-> Could not add new Action To ActiveActions, Action not valid"));
		return;
	}
	ActiveActions.Add(NewAction);
}

void UAS_ActionSystemComponent::RemoveActiveAction(UAS_Action* ActionToRemove)
{
	if (!ActionToRemove) {
		UE_LOG(LogTemp, Warning, TEXT("UAS_ActionSystemComponent::RemoveActiveAction -> Could not remove Action from ActiveActions, Action not valid"));
		return;
	}

	if (CurrentAction == ActionToRemove) {
		RemoveCurrentAction();
	}
	ActiveActions.Remove(ActionToRemove);
}

bool UAS_ActionSystemComponent::IsActionRunning(const FGameplayTag ActionToCheck) const
{
	// @TODO Sprawdz czy musz� sprawdza� czy action jest "valid","istnieje" czy nie jest to przypadkiem oczywiste bo tworze przecierz obiekt kt�ry musi by� tego typu? 
	for (const UAS_Action* Action : ActiveActions) {
		if (Action && Action->ActionTag == ActionToCheck) { return true; }
	}
	return false;
}

bool UAS_ActionSystemComponent::StartActionByTag(FGameplayTag ActionToStart)
{
	for (UAS_Action* Action : AvaliableActions) {
		UE_LOG(LogTemp, Warning, TEXT("We have the avaliable acions"));

		if (Action && Action->ActionTag == ActionToStart) {
			UE_LOG(LogTemp, Warning, TEXT("We found the right action to start "));

			if ( !Action->CanStartAction(ActionContext) ) { continue; }

			UE_LOG(LogTemp, Warning, TEXT("action as able to start"));
			Action->StartAction(ActionContext);
			GrantActiveTags(Action->GrantedTags);
			AddActiveAction(Action);
			OnActionStart.Broadcast(ActionToStart);
			return true;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("UAS_ActionSystemComponent::StartActionByTag -> action was not able to start"));
	return false;
}

bool UAS_ActionSystemComponent::StopActionByTag(FGameplayTag ActionToStop)
{
	for (UAS_Action* Action : AvaliableActions) {
		if (Action->ActionTag == ActionToStop) {
			if (!Action->CanStopAction(ActionContext)) { continue; }
			Action->StopAction(ActionContext);
			RemoveActiveTags(Action->GrantedTags);
			RemoveActiveAction(Action);
			OnActionStop.Broadcast(ActionToStop);
			return true;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("UAS_ActionSystemComponent::StopActionByTag -> action was not able to stop"));
	return false;

}

bool UAS_ActionSystemComponent::StartActionByClass(const TSubclassOf<UAS_Action> ActionClass)
{
	if (ActionClass) {
		for (UAS_Action* Action : AvaliableActions) {
			if (Action->GetClass() == ActionClass)

				if (Action->CanStartAction(ActionContext)) {

					Action->StartAction(ActionContext);
					GrantActiveTags(Action->GrantedTags);
					AddActiveAction(Action);
					OnActionStart.Broadcast(Action->ActionTag);
					return true;
				}
		}
	}
	return false;

}



bool UAS_ActionSystemComponent::StopActionByClass(TSubclassOf<UAS_Action> ActionClass)
{
	if (ActionClass) {
		for (UAS_Action* Action : AvaliableActions) {
			if (Action->GetClass() == ActionClass)
			{
				if (Action->CanStopAction(ActionContext)) {
					Action->StopAction(ActionContext);
					RemoveActiveTags(Action->GrantedTags);
					RemoveActiveAction(Action);
					OnActionStop.Broadcast(Action->ActionTag);
					return true;
				}
			}
		}
	}
	return false;
}

bool UAS_ActionSystemComponent::StopCurrentAction()
{
	if (CurrentAction && ActiveActions.Contains(CurrentAction)) {
		if (!CurrentAction->CanStopAction(ActionContext))
		{
			return false;
		}
		CurrentAction->StopAction(ActionContext);
		RemoveActiveTags(CurrentAction->GrantedTags);
		//if i want to free the current action this need to happend before remove active action on currentAction if not we will have a crash there is another way i can do this but it's late have to do for now. 
		OnActionStop.Broadcast(CurrentAction->ActionTag);
		RemoveActiveAction(CurrentAction);
		return true;
	}
	return false;
}

float UAS_ActionSystemComponent::PlayActionMontage(UAnimMontage* MontageToPlay, const float PlayRate, const FName StartSection)
{
	if (!MontageToPlay) {
		UE_LOG(LogTemp, Warning, TEXT("Montage Not Valid"));
		return 0.0f;
	}
	if (ActionContext.AvatarCharater) {
		return ActionContext.AvatarCharater->PlayAnimMontage(MontageToPlay, PlayRate, StartSection);
	}

	return 0.0f;
}
