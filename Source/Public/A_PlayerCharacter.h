// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AlchemystCharacter.h"
#include "A_Gatherable.h"
#include "A_PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMYST_API AA_PlayerCharacter : public AAlchemystCharacter
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Component")
    class USphereComponent* InteractableCollision;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Component")
    class USceneComponent* ThrowPosition;

    TArray<class AInteractible*> ObjectsInRange;

public:
    AA_PlayerCharacter();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Player")
    class UItem* EquippedItem;

    UItem* GetEquippedItem() const
    {
        return EquippedItem;
    }

    UFUNCTION(BlueprintCallable)
    void SetEquippedItem(UItem* NewEquippedItem);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Component")
    class UInventoryComponent* Inventory;

    void MoveUp(float Value);
    void MoveRight(float Value);

    UFUNCTION()
    void OnInteractableOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                               const FHitResult& SweepResult);

    UFUNCTION()
    void OnEndInteractableOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    bool IsInRange(AInteractible* HitObject);

    FVector GetThrowPos();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Health")
    float Health;

    UFUNCTION(BlueprintCallable, Category= "Items")
    void UseItem(class UItem* Item);

    void RemoveItem(class UItem* Item);
};
