// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Potions/Potion.h"

#include "Kismet/GameplayStatics.h"

UPotion::UPotion()
{
    static ConstructorHelpers::FObjectFinder<UDataTable> PotionColorsDataObject(
        TEXT("DataTable'/Game/DataTable/PotionColorTable.PotionColorTable'"));
    if (PotionColorsDataObject.Succeeded())
    {
        Colors = PotionColorsDataObject.Object;
    }

    ThrowAbilityIndex = Throw_Explosive;
}

FLinearColor UPotion::GetLiquidColor()
{
    static const FString ContextString(TEXT("Potion Color Context"));
    return Colors->FindRow<FPotionColorRow>(UEnum::GetValueAsName(LiquidColor), ContextString, true)->ColorValue;
}

void UPotion::CalculateEffect()
{
}

void UPotion::InitWithOtherPotion(UPotion* OtherPotion)
{
    Taste = OtherPotion->Taste;
    LiquidColor = OtherPotion->LiquidColor;
    ThrowAbility = OtherPotion->ThrowAbility;

    const FString Path = "/Game/Textures/Thumbnails/Potions/Potion_" + UEnum::GetValueAsString(LiquidColor);
    UTexture2D* LoadedTb = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *(Path)));
    if (LoadedTb)
    {
        Thumbnail = LoadedTb;
    }
}

void UPotion::SetLiquidColor(TEnumAsByte<EPotionColors> NewColor)
{
    LiquidColor = NewColor;
}
