// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AlchemystCharacter.h"
#include "A_Enemy.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMYST_API AA_Enemy : public AAlchemystCharacter
{
	GENERATED_BODY()

public:
    AA_Enemy();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Component")
    class UInventoryComponent* InventoryComponent;

    UPROPERTY(EditAnywhere)
    TSubclassOf<class ALootBox> LootBoxClass;
    
    virtual void OnDeath() override;
};
