// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Item.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class ALCHEMYST_API UItem : public UObject
{
	GENERATED_BODY()

public:
    UItem();

    virtual class UWorld* GetWorld() const override { return World; }

    UPROPERTY(Transient)
    class UWorld* World;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= "Item")
    FText UseActionText;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= "Item")
    FText InteractActionText;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= "Item")
    class UStaticMesh* DroppedMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= "Item")
    class UTexture2D* Thumbnail;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= "Item")
    FText ItemDisplayName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= "Item", meta = (MultiLine = true))
    FText ItemDescription;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= "Item", meta = (ClampMin = 0.0))
    float Weight;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UInventoryComponent* OwningInventory;

    virtual void Use(class AA_PlayerCharacter* Character);

    UFUNCTION(BlueprintImplementableEvent)
    void OnUse(class AA_PlayerCharacter* Character);
};
