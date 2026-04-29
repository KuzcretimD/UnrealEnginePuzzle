// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AS_ActionTypes.generated.h"

USTRUCT(BlueprintType)
struct FAS_ActionContext {
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UObject> ContextObject;
	UPROPERTY()
	TObjectPtr<AActor> AvatarActor;
	UPROPERTY()
	TObjectPtr<ACharacter> AvatarCharater;
	UPROPERTY()
	TObjectPtr<USkeletalMeshComponent> AvatarMesh;
	UPROPERTY()
	TObjectPtr<UAnimInstance> AvatarAnimInstance;
};


USTRUCT(BlueprintType)
struct FAS_ActionMontage {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	class UAnimMontage* ActionMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	float PlayRate = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	bool bRandomMontageSection = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	TArray<FName> MontageSectionNames;

	FAS_ActionMontage() :ActionMontage(nullptr), PlayRate(1.f), bRandomMontageSection(false), MontageSectionNames({}) {}

	bool  bValidMontage() { return ActionMontage != nullptr; }
	int32 GetLastSectionIndex()const { return MontageSectionNames.Num() - 1; }
	int32 GetRandomIndex()const {
		if (MontageSectionNames.IsEmpty()) {
			return -1;
		}

		const int32 RandIndex = FMath::RandRange(0, GetLastSectionIndex());

		if (!MontageSectionNames.IsValidIndex(RandIndex)) {
			return -1;
		}
		return RandIndex;
	}
	FName GetMontageSectionName(const int32 SectionIndex)const {
		int32 SelectedIndex = SectionIndex;
		if (MontageSectionNames.IsEmpty()) {
			return NAME_None;
		}

		//This might be wrong check to be sure, might give some random index but not nessesarly the index i want to get if you get what i mean...
		//-Alan
		if (bRandomMontageSection) { SelectedIndex = GetRandomIndex(); }

		if (!MontageSectionNames.IsValidIndex(SelectedIndex)) { return NAME_None; }
		return MontageSectionNames[SelectedIndex];
	}

};

/**
 * 
 */
UCLASS()
class GAMEANIMATIONSAMPLEBASIC_API UAS_ActionTypes : public UObject
{
	GENERATED_BODY()
	
};
