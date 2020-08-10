// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactible.h"
#include "Blueprint/UserWidget.h"
#include "CraftingStation.generated.h"

UCLASS()
class ALCHEMYST_API ACraftingStation : public AInteractible
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACraftingStation();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* Mesh;

public:
	virtual void Interact(AA_PlayerCharacter* InteractingPlayer) override;
};
