// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactible.h"
#include "Item.h"
#include "A_Gatherable.generated.h"

UCLASS()
class ALCHEMYST_API AA_Gatherable : public AInteractible
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AA_Gatherable();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Item")
    TSubclassOf<UItem> ItemClass;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Item")
    UItem* Item;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 UsesLeft;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Component")
    class UStaticMeshComponent* Mesh;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    virtual void Interact(AA_PlayerCharacter* InteractingPlayer) override;

    UFUNCTION(BlueprintCallable)
    void OnItemDropped(UItem* DroppedItem);

    UFUNCTION(BlueprintImplementableEvent)
    void OnItemDroppedBP(UItem* DroppedItem);
};
