// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/AlchGameplayAbility.h"
#include "GA_Potion_Consume.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMYST_API UGA_Potion_Consume : public UAlchGameplayAbility
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Potion")
	class UPotion* Potion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Potion")
	class UNiagaraSystem* ConsumeFX;

	class AAlchemystPlayerController* OwningPlayerController;
	class AA_PlayerCharacter* OwningPlayerCharacter;

	public:
	UGA_Potion_Consume();

	virtual void ActivateAbility(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo,
                                 FGameplayAbilityActivationInfo ActivationInfo,
                                 const FGameplayEventData* TriggerEventData) override;

	virtual void InputReleased(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                               FGameplayAbilityActivationInfo ActivationInfo) override;

	virtual void CancelAbility(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                               FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

};
