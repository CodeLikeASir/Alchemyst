// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactible.generated.h"

UCLASS()
class ALCHEMYST_API AInteractible : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AInteractible();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Component")
    class UWidgetComponent* InteractUI;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Component")
    class USphereComponent* Trigger;

public:
    virtual void ToggleInteractUI(bool bIsVisible);

    virtual void Interact(class AA_PlayerCharacter* InteractingPlayer);

    UFUNCTION(BlueprintImplementableEvent, Category="Item")
    void InteractBP(class AA_PlayerCharacter* InteractingPlayer);
};
