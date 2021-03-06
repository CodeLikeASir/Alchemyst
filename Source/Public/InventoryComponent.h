// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "A_PlayerCharacter.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALCHEMYST_API UInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UInventoryComponent();

public:
    // Called when the game starts
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable)
    bool AddItem(class UItem* Item);

    UFUNCTION(BlueprintCallable)
    bool RemoveItem(class UItem* Item);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    AA_PlayerCharacter* OwningPlayer;

    UPROPERTY(EditDefaultsOnly, Instanced)
    TArray<class UItem*> DefaultItems;

    UPROPERTY(EditDefaultsOnly, Category= "Inventory")
    int32 Capacity;

    UPROPERTY(BlueprintAssignable, Category= "Inventory")
    FOnInventoryUpdated OnInventoryUpdated;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Items")
    TArray<class UItem*> Items;

    UFUNCTION(BlueprintCallable)
    void InitItems(TArray<class UItem*> NewItems);

    UFUNCTION(BlueprintCallable)
    bool IsEmpty()
    {
        return Items.Num() == 0;
    }
};
