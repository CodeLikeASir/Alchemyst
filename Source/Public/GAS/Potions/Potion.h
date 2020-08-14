// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Engine/DataTable.h"
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

USTRUCT(BlueprintType)
struct FPotionColorRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLinearColor ColorValue;
};

UENUM(BlueprintType)
enum EPotionColors
{
	Green, Red, Blue, Grey, Purple
};

UENUM(BlueprintType)
enum EPotionAbilities
{
	Throw_Explosive
};

UCLASS(BlueprintType)
class ALCHEMYST_API UPotion : public UItem
{
	GENERATED_BODY()

	UPROPERTY()
	UDataTable* Colors;
	
protected:
	UPotion();
	
	UPROPERTY(EditAnywhere, Category= "Potion")
	TEnumAsByte<EPotionColors> LiquidColor;
	
public:
	UFUNCTION(BlueprintCallable)
	FLinearColor GetLiquidColor();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= "Potion")
	FTasteStruct Taste;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Ability")
	TSubclassOf<class UAlchGameplayAbility> ThrowAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Ability")
	TEnumAsByte<EPotionAbilities> ThrowAbilityIndex;
	
	UFUNCTION(BlueprintCallable)
	void CalculateEffect();

	UFUNCTION(BlueprintCallable)
	void InitWithOtherPotion(UPotion* OtherPotion);

	UFUNCTION(BlueprintCallable)
	void SetLiquidColor(TEnumAsByte<EPotionColors> NewColor);
};
