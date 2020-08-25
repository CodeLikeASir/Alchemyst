// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Potions/GA_Potion_Consume.h"
#include "A_PlayerCharacter.h"
#include "AlchemystPlayerController.h"
#include "Abilities/GameplayAbility.h"
#include "GAS/AlchGameplayAbility.h"
#include "GAS/Potions/Potion.h"

UGA_Potion_Consume::UGA_Potion_Consume()
{
}

void UGA_Potion_Consume::ActivateAbility(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo,
    FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, OwnerInfo, ActivationInfo, TriggerEventData);

    OwningPlayerController = CastChecked<AAlchemystPlayerController>(OwnerInfo->PlayerController.Get());
    OwningPlayerCharacter = CastChecked<AA_PlayerCharacter>(OwningPlayerController->GetCharacter());
    Potion = Cast<UPotion>(OwningPlayerCharacter->GetEquippedItem());

    if(!Potion)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Player tried to throw without potion")));
        return;
    }
}

void UGA_Potion_Consume::InputReleased(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
    FGameplayAbilityActivationInfo ActivationInfo)
{
    if (ActorInfo != nullptr && ActorInfo->AvatarActor != nullptr)
    {
        CancelAbility(Handle, ActorInfo, ActivationInfo, true);
    }
}

void UGA_Potion_Consume::CancelAbility(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
    FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
    Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}
