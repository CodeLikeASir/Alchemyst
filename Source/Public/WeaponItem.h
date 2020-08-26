// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "WeaponItem.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMYST_API UWeaponItem : public UItem
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Weapon")
    TSubclassOf<class AAlch_RangedWeapon> WeaponActor;

    virtual void Use(AA_PlayerCharacter* Character) override;
};
