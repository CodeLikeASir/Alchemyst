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

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Ability")
    FGameplayTag AbilityTag;

    FGameplayTag GetAbilityTag() const
    {
        return AbilityTag;
    }

    // Abilities with this set will automatically activate when the input is pressed
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
    EGDAbilityInputID AbilityInputID = EGDAbilityInputID::None;
};

/** A "processed" version of RPGGameplayEffectContainer that can be passed around and eventually applied */
USTRUCT(BlueprintType)
struct FAlchGameplayEffectContainerSpec
{
    GENERATED_BODY()

public:
    FAlchGameplayEffectContainerSpec() {}

    /** Computed target data */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
    FGameplayAbilityTargetDataHandle TargetData;

    /** List of gameplay effects to apply to the targets */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
    TArray<FGameplayEffectSpecHandle> TargetGameplayEffectSpecs;

    /** Returns true if this has any valid effect specs */
    bool HasValidEffects() const;

    /** Returns true if this has any valid targets */
    bool HasValidTargets() const;

    /** Adds new targets to target data */
    void AddTargets(const TArray<FHitResult>& HitResults, const TArray<AActor*>& TargetActors);
};