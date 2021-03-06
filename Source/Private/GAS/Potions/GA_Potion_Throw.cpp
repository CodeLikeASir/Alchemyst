// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Potions/GA_Potion_Throw.h"
#include "AlchemystPlayerController.h"
#include "GAS/Potions/Potion_ThrownActor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "A_PlayerCharacter.h"
#include "GAS/Potions/Potion.h"

UGA_Potion_Throw::UGA_Potion_Throw()
{
    AbilityInputID = EGDAbilityInputID::Throw;

    AbilityTag = FGameplayTag::RequestGameplayTag(FName("Potion.Throw"));
}

void UGA_Potion_Throw::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* OwnerInfo,
                                       const FGameplayAbilityActivationInfo ActivationInfo,
                                       const FGameplayEventData* TriggerEventData)
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
    
    SpawnPotionProjectile();
}

void UGA_Potion_Throw::InputReleased(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo,
                                     const FGameplayAbilityActivationInfo ActivationInfo)
{
    if (ActorInfo != nullptr && ActorInfo->AvatarActor != nullptr)
    {
        CancelAbility(Handle, ActorInfo, ActivationInfo, true);
    }
}

void UGA_Potion_Throw::CancelAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo,
                                     const FGameplayAbilityActivationInfo ActivationInfo,
                                     bool bReplicateCancelAbility)
{
    Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UGA_Potion_Throw::SpawnPotionProjectile()
{
    FVector CursorHitPos = OwningPlayerController->GetCursorHitPos();
    FVector ThrowPos = OwningPlayerCharacter->GetThrowPos();
    FVector ShootNormal = CursorHitPos - ThrowPos;

    // Rotate player in throw direction
    FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(OwningPlayerCharacter->GetActorLocation(),
                                                                  CursorHitPos);
    OwningPlayerCharacter->SetActorRotation(FRotator(
        OwningPlayerCharacter->GetControlRotation().Pitch,
        NewRotation.Yaw,
        OwningPlayerCharacter->GetControlRotation().Roll));

    FRotator SpawnRotation = ShootNormal.Rotation();
    SpawnRotation.Pitch += 45.f;
    FTransform SpawnTransform = FTransform(SpawnRotation, ThrowPos);

    //Set Spawn Collision Handling Override
    FActorSpawnParameters ActorSpawnParams;
    ActorSpawnParams.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    APotion_ThrownActor* ThrownPotion = Cast<APotion_ThrownActor>(
        UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ThrownPotionClass, SpawnTransform));
    if (ThrownPotion != nullptr)
    {
        ThrownPotion->InitThrownPotion(Potion, OwningPlayerCharacter->GetPlayerSpeed());

        UGameplayStatics::FinishSpawningActor(ThrownPotion, SpawnTransform);
    }

    if(Potion)
        OwningPlayerCharacter->RemoveItem(Potion);
    else
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Potion reference missing")));
}
