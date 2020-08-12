// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

#include "A_Gatherable.h"
#include "Kismet/GameplayStatics.h"

UItem::UItem()
{
    Weight = 1.f;
    ItemDisplayName = FText::FromString("Item");
    UseActionText = FText::FromString("Use");
    InteractActionText = FText::FromString("Interact");
}

void UItem::Use(AA_PlayerCharacter* Character)
{
}