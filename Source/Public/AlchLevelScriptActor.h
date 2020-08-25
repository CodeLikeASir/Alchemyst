// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "AlchLevelScriptActor.generated.h"

#define ColorVal(Color) ((Color.R + Color.G + Color.B) / 765.f)
/**
 * 
 */
UCLASS()
class ALCHEMYST_API AAlchLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()

protected:
    UFUNCTION(BlueprintCallable, Category= "LevelGeneration")
    TArray<FVector2D> GenerateLevel(UTexture2D* NoiseTexture, int Granularity);
};
