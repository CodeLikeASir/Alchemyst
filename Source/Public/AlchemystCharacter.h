// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GAS/AlchGameplayAbility.h"
#include "AlchemystCharacter.generated.h"

#define CastToInt( val ) ( static_cast<uint8>(val) )

UCLASS(Blueprintable)
class AAlchemystCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AAlchemystCharacter();

private:
    float DefaultSpeed;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
    TSubclassOf<class UAlchGameplayAbility> ProjectileAbility;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Character")
    class UCharacterAttributes* Attributes;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "GAS")
    class UAbilitySystemComponent* AbilitySystemComponent;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS")
    TArray<TSubclassOf<class UAlchGameplayAbility>> CharacterAbilities;

    void AddCharacterAbilities();

    void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    UAbilitySystemComponent* GetAbilitySystemComponent() const
    {
        return AbilitySystemComponent;
    }

    virtual void PossessedBy(AController* NewController) override;

    void SetupInput(class UInputComponent* PlayerInputComponent);

    void GrantAbility(TSubclassOf<class UAlchGameplayAbility> Ability);
    void RemoveAbility(TSubclassOf<class UAlchGameplayAbility> Ability);

    FGameplayAbilitySpecHandle PotionThrowHandle;

    float GetPlayerSpeed();

    UFUNCTION(BlueprintCallable)
    float GetHealthPercentage();

    UFUNCTION(BlueprintCallable)
    void Heal(float Value);

    virtual void OnDeath();

    UFUNCTION()
    virtual void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
                                     class AController* InstigatedBy, AActor* DamageCauser);

    UFUNCTION(BlueprintImplementableEvent)
    void HandleTakeAnyDamageBP(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
                                     class AController* InstigatedBy, AActor* DamageCauser);

    UFUNCTION(BlueprintCallable)
    void ApplyGameplayEffect(class UGameplayEffect* GameplayEffect);

    UFUNCTION(BlueprintCallable)
    USkeletalMeshComponent* GetSkeletalMesh();

    bool bIsStunned;
    virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
};
