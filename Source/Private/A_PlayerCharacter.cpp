// Fill out your copyright notice in the Description page of Project Settings.


#include "A_PlayerCharacter.h"

#include "A_Gatherable.h"
#include "Item.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SphereComponent.h"
#include "InventoryComponent.h"
#include "GAS/Potions/Potion.h"

AA_PlayerCharacter::AA_PlayerCharacter()
{
    InteractableCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Interactable"));
    InteractableCollision->InitSphereRadius(150.f);
    InteractableCollision->OnComponentBeginOverlap.AddDynamic(this, &AA_PlayerCharacter::OnInteractableOverlap);
    InteractableCollision->OnComponentEndOverlap.AddDynamic(this, &AA_PlayerCharacter::OnEndInteractableOverlap);
    InteractableCollision->SetupAttachment(RootComponent);

    Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
    Inventory->Capacity = 20;

    ThrowPosition = CreateDefaultSubobject<USphereComponent>(TEXT("ThrowPosition"));
    ThrowPosition->SetupAttachment(RootComponent);

    Health = 100.f;
}

void AA_PlayerCharacter::SetEquippedItem(UItem* NewEquippedItem)
{
    if (UPotion* Potion = Cast<UPotion>(EquippedItem))
    {
        if (Potion->ThrowAbility)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Removed %s"), *Potion->ThrowAbility->GetName()));
            RemoveAbility(Potion->ThrowAbility);
        }
    }
    
    EquippedItem = NewEquippedItem;

    if (UPotion* Potion = Cast<UPotion>(NewEquippedItem))
    {
        if (Potion->ThrowAbility)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Grant %s"), *Potion->ThrowAbility->GetName()));
            GrantAbility(Potion->ThrowAbility);
        }
    }
}

void AA_PlayerCharacter::MoveUp(float Value)
{
    AddMovementInput(UKismetMathLibrary::GetForwardVector(FRotator(0, GetControlRotation().Yaw, 0)), Value);
}

void AA_PlayerCharacter::MoveRight(float Value)
{
    AddMovementInput(UKismetMathLibrary::GetRightVector(FRotator(0, GetControlRotation().Yaw, 0)), Value);
}

void AA_PlayerCharacter::OnInteractableOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                               const FHitResult& SweepResult)
{
    if (AInteractible* Interactible = Cast<AInteractible>(OtherActor))
    {
        Interactible->ToggleInteractUI(true);
        ObjectsInRange.Add(Interactible);
    }
}

void AA_PlayerCharacter::OnEndInteractableOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (AInteractible* Interactible = Cast<AInteractible>(OtherActor))
    {
        Interactible->ToggleInteractUI(false);
        ObjectsInRange.Remove(Interactible);
    }
}

bool AA_PlayerCharacter::IsInRange(AInteractible* HitObject)
{
    return ObjectsInRange.Contains(HitObject);
}

FVector AA_PlayerCharacter::GetThrowPos()
{
    return ThrowPosition->GetComponentLocation();
}

void AA_PlayerCharacter::UseItem(UItem* Item)
{
    if (Item)
    {
        Item->Use(this);
        Item->OnUse(this); // BP event
    }
}

void AA_PlayerCharacter::RemoveItem(UItem* Item)
{
    if(Item == EquippedItem)
    {

        if (UPotion* Potion = Cast<UPotion>(EquippedItem))
        {
            if (Potion->ThrowAbility)
            {
                RemoveAbility(Potion->ThrowAbility);
            }
        }
        
        EquippedItem = nullptr;
    }

    Inventory->RemoveItem(Item);
}
