// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactible.h"
#include "Item.h"
#include "LootBox.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMYST_API ALootBox : public AInteractible
{
	GENERATED_BODY()

public:
    ALootBox();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
    class UInventoryComponent* InventoryComponent;

    void InitLoot(TArray<class UItem*> Items);
};
