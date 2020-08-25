// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AttributeSet/CharacterAttributes.h"
#include "AbilitySystemComponent.h"
#include "AlchemystPlayerController.h"
#include "A_PlayerCharacter.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

void UCharacterAttributes::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributes, Health, OldHealth);
}

void UCharacterAttributes::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributes, MaxHealth, OldMaxHealth);
}

float UCharacterAttributes::GetHealthPercentage()
{
    return Health.GetCurrentValue() / MaxHealth.GetCurrentValue();
}

void UCharacterAttributes::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
}

void UCharacterAttributes::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);

    FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
    UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
    const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();
    FGameplayTagContainer SpecAssetTags;
    Data.EffectSpec.GetAllAssetTags(SpecAssetTags);

    // Get the Target actor, which should be our owner
    AActor* TargetActor = nullptr;
    AAlchemystPlayerController* TargetController = nullptr;
    AAlchemystCharacter* TargetCharacter = nullptr;
    if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
    {
        TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
        //TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
        TargetCharacter = Cast<AAlchemystCharacter>(TargetActor);
    }
    
    if(Data.EvaluatedData.Attribute == GetHealthAttribute())
    {
        if(Health.GetCurrentValue() <= 0.f)
        {
            TargetCharacter->OnDeath();
        }
    }
}

void UCharacterAttributes::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributes, Health, COND_None, REPNOTIFY_Always);
}
