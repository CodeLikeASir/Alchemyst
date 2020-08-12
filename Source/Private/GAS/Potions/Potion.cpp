// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Potions/Potion.h"

void UPotion::CalculateEffect()
{
}

void UPotion::InitWithOtherPotion(UPotion* OtherPotion)
{
    Taste = OtherPotion->Taste;
}
