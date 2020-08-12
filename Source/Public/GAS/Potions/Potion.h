// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "UObject/NoExportTypes.h"
#include "Potion.generated.h"

USTRUCT(BlueprintType)
struct FTasteStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Potion/Taste")
	float Sweetness;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Potion/Taste")
	float Sourness;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Potion/Taste")
	float Saltiness;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Potion/Taste")
	float Bitterness;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Potion/Taste")
	float Spiciness;

	FTasteStruct()
	{
		Sweetness = 0.f;
		Sourness = 0.f;
		Saltiness = 0.f;
		Bitterness = 0.f;
		Spiciness = 0.f;
	}

	FTasteStruct& operator+=(FTasteStruct& OtherTaste)
	{
		Sweetness += OtherTaste.Sweetness;
		Sourness += OtherTaste.Sourness;
		Saltiness += OtherTaste.Saltiness;
		Bitterness += OtherTaste.Bitterness;
		Spiciness += OtherTaste.Spiciness;

		return *this;
	}
};

UCLASS(BlueprintType)
class ALCHEMYST_API UPotion : public UItem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= "Potion")
	FTasteStruct Taste;
	
	UFUNCTION(BlueprintCallable)
	void CalculateEffect();

	UFUNCTION(BlueprintCallable)
	void InitWithOtherPotion(UPotion* OtherPotion);
};
