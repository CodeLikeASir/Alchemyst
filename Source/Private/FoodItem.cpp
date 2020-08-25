// Fill out your copyright notice in the Description page of Project Settings.


#include "FoodItem.h"

#include "A_PlayerCharacter.h"
#include "InventoryComponent.h"

void UFoodItem::Use(AA_PlayerCharacter* Character)
{
    if (Character)
    {
        Character->Heal(HealthToHeal);

        if (OwningInventory)
        {
            OwningInventory->RemoveItem(this);
        }
    }
}
