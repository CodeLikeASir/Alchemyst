// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Potion.h"
#include "UObject/NoExportTypes.h"
#include "Plant.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMYST_API UPlant : public UItem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= "Potion")
	FTasteStruct Taste;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Plant")
	UMaterialInterface* PlantMaterial;
};
