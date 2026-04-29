// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/AS_Action.h"
#include "ActionSystem/AS_ActionSystemComponent.h"

void UAS_Action::StartAction_Implementation(const FAS_ActionContext& ActionContext)
{
}

void UAS_Action::StopAction_Implementation(const FAS_ActionContext& ActionContext)
{
}

bool UAS_Action::CanStartAction_Implementation(const FAS_ActionContext& ActionContext) const
{
	if (UAS_ActionSystemComponent* ActionComponent = GetActionSystemComponent())
	{
		
		UE_LOG(LogTemp, Warning, TEXT("I have the Action Component alright checking if satifytargetRequirements are alright"));
		return SatisfyTargetRequirements(ActionComponent->ActiveGameplayTags);
	}
	return false;
}

bool UAS_Action::CanStopAction_Implementation(const FAS_ActionContext& ActionContext) const
{
	return true;
}

float UAS_Action::PlayActionMotage_Implementation(UAnimMontage* MontageToPlay, const float PlayRate, const FName StartSection)
{

	if (!ActionMontage.bValidMontage()) {
		return 0.f;
	}

	if (UAS_ActionSystemComponent* ActionComp = GetActionSystemComponent()) {
		ActionComp->PlayActionMontage(MontageToPlay, PlayRate, StartSection);
	}
	return 0.f;
}

bool UAS_Action::OverrideCurrentAction()
{
	if (UAS_ActionSystemComponent* ActionComp = GetActionSystemComponent()) {
		ActionComp->StopCurrentAction();
		ActionComp->SetCurrentAction(this);
		return true;
	}
	return false;
}

bool UAS_Action::SatisfyTargetRequirements(const FGameplayTagContainer& ActiveGameplayTags) const
{
	if (!ActiveGameplayTags.HasAny(BlockedTags)) {
		UE_LOG(LogTemp, Warning, TEXT("REQUIREMENTS ARE ALRIGHT"));
	}
	return !ActiveGameplayTags.HasAny(BlockedTags);
}

UAS_ActionSystemComponent* UAS_Action::GetActionSystemComponent() const
{
	return Cast<UAS_ActionSystemComponent>(GetOuter());

}


