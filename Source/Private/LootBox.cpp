// Fill out your copyright notice in the Description page of Project Settings.


#include "LootBox.h"
#include "Item.h"
#include "InventoryComponent.h"

ALootBox::ALootBox()
{
    InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
    
    InteractText = FText::FromString("Loot");
}

void ALootBox::InitLoot(TArray<class UItem*> Items)
{
    InventoryComponent->InitItems(Items);
}
