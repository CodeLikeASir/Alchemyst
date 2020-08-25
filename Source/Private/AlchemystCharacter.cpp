// Copyright Epic Games, Inc. All Rights Reserved.

#include "AlchemystCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "AbilitySystemComponent.h"
#include "Alchemyst.h"
#include "GameplayAbilitySpec.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "GAS/AlchGameplayAbility.h"
#include "GAS/AttributeSet/CharacterAttributes.h"
#include "GameplayEffect.h"

AAlchemystCharacter::AAlchemystCharacter()
{
    // Set size for player capsule
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    // Don't rotate character to camera direction
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Configure character movement
    GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
    GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
    GetCharacterMovement()->bConstrainToPlane = true;
    GetCharacterMovement()->bSnapToPlaneAtStart = true;

    AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));

    Attributes = CreateDefaultSubobject<UCharacterAttributes>(TEXT("CharacterAttributes"));
    Attributes->InitMaxHealth(100.f);
    Attributes->InitHealth(100.f);

    OnTakeAnyDamage.AddDynamic(this, &AAlchemystCharacter::HandleTakeAnyDamage);

    AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("CC.Petrified")), EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AAlchemystCharacter::StunTagChanged);

    DefaultSpeed = 600.f; // GetCharacterMovement()->GetMaxSpeed();
}

void AAlchemystCharacter::AddCharacterAbilities()
{
    // Grant abilities, but only on the server	
    if (GetLocalRole() != ROLE_Authority || !IsValid(AbilitySystemComponent))
    {
        return;
    }

    for (TSubclassOf<UAlchGameplayAbility>& StartupAbility : CharacterAbilities)
    {
        //GetAbilityLevel(StartupAbility.GetDefaultObject()->AbilityID)
        AbilitySystemComponent->GiveAbility(
            FGameplayAbilitySpec(StartupAbility, 1, static_cast<int32>(EGDAbilityInputID::Test), this));
    }

    //AbilitySystemComponent->bCharacterAbilitiesGiven = true;
}

void AAlchemystCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    // set up gameplay key bindings
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    UAbilitySystemComponent* AbilitySystem = GetAbilitySystemComponent();
    if (AbilitySystem)
    {
        AbilitySystem->BindAbilityActivationToInputComponent(InputComponent,
                                                             FGameplayAbilityInputBinds(
                                                                 "ConfirmInput", "CancelInput", "EGDAbilityInputID"));
        AddCharacterAbilities();
    }
    //MyCharacter->SetupInput(InputComponent);
}

void AAlchemystCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    if (AbilitySystemComponent)
    {
        AbilitySystemComponent->InitAbilityActorInfo(this, this);
    }

    // ASC MixedMode replication requires that the ASC Owner's Owner be the Controller.
    SetOwner(NewController);
}

void AAlchemystCharacter::SetupInput(UInputComponent* PlayerInputComponent)
{
    // Bind to AbilitySystemComponent
    //AbilitySystemComponent->BindAbilityActivationToInputComponent(PlayerInputComponent, FGameplayAbilityInputBinds(
    //	FString("ConfirmTarget"), FString("CancelTarget"), FString("EGDAbilityInputID"),
    //	static_cast<int32>(EGDAbilityInputID::Confirm), static_cast<int32>(EGDAbilityInputID::Cancel)));
}

void AAlchemystCharacter::GrantAbility(TSubclassOf<class UAlchGameplayAbility> Ability)
{
    // Grant abilities, but only on the server	
    if (GetLocalRole() != ROLE_Authority || !IsValid(AbilitySystemComponent))
    {
        return;
    }

    FGameplayAbilitySpecHandle Handle = AbilitySystemComponent->GiveAbility(
        FGameplayAbilitySpec(Ability, 1, static_cast<int32>(Ability.GetDefaultObject()->AbilityInputID), this));

    FGameplayTag AblityTag = Ability.GetDefaultObject()->GetAbilityTag();
    if(AblityTag == FGameplayTag::RequestGameplayTag(FName("Potion.Throw")))
    {
        PotionThrowHandle = Handle;
    }
}

void AAlchemystCharacter::RemoveAbility(TSubclassOf<UAlchGameplayAbility> Ability)
{
    if (GetLocalRole() != ROLE_Authority || !IsValid(AbilitySystemComponent))
    {
        return;
    }

    FGameplayTag AblityTag = Ability.GetDefaultObject()->GetAbilityTag();
    if(AblityTag == FGameplayTag::RequestGameplayTag(FName("Potion.Throw")))
    {
        AbilitySystemComponent->ClearAbility(PotionThrowHandle);
    }
}

float AAlchemystCharacter::GetPlayerSpeed()
{
    return GetVelocity().Size();
}

float AAlchemystCharacter::GetHealthPercentage()
{
    return Attributes->GetHealthPercentage();
}

void AAlchemystCharacter::Heal(float Value)
{
    Attributes->SetHealth(Attributes->GetHealth() + Value);
}

void AAlchemystCharacter::OnDeath()
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s died."), *GetName()));

    Destroy();
}

void AAlchemystCharacter::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                              AController* InstigatedBy, AActor* DamageCauser)
{
    HandleTakeAnyDamageBP(DamagedActor, Damage, DamageType, InstigatedBy, DamageCauser);
    
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s took %f points of damage"), *DamagedActor->GetName(), Damage));
    Attributes->Health.SetCurrentValue(Attributes->Health.GetCurrentValue() - Damage);

    if(Attributes->Health.GetCurrentValue() <= 0.f)
    {
        Destroy();
    }
}

void AAlchemystCharacter::ApplyGameplayEffect(UGameplayEffect* GameplayEffect)
{
    // (const UGameplayEffect *GameplayEffect, float Level, const FGameplayEffectContextHandle& EffectContext, FPredictionKey PredictionKey = FPredictionKey())
    FGameplayEffectContextHandle Handle = FGameplayEffectContextHandle();
    AbilitySystemComponent->ApplyGameplayEffectToSelf(GameplayEffect, 0.f, Handle);
}

USkeletalMeshComponent* AAlchemystCharacter::GetSkeletalMesh()
{
    return GetMesh();
}

void AAlchemystCharacter::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Pew Pew Pew")));
    
    if(CallbackTag == FGameplayTag::RequestGameplayTag(FName("CC.Petrified")))
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Stun tag changed")));
        AbilitySystemComponent->CancelAbilities();

        bIsStunned = !bIsStunned;
        GetCharacterMovement()->MaxWalkSpeed = bIsStunned ? 0.f : 600.f; //DefaultSpeed;
    }
}
