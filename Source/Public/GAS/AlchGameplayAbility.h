// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Alchemyst.h"
#include "AlchGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMYST_API UAlchGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UAlchGameplayAbility();
	
	// Abilities with this set will automatically activate when the input is pressed
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EGDAbilityInputID AbilityInputID = EGDAbilityInputID::None;
};
