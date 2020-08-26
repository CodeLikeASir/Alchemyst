// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAlchemyst, Log, All);

UENUM(BlueprintType)
enum class EGDAbilityInputID : uint8
{
    // 0 None
    None UMETA(DisplayName = "None"),
    // 1 Confirm
    Confirm UMETA(DisplayName = "Confirm"),
    // 2 Cancel
    Cancel UMETA(DisplayName = "Cancel"),
    // 4 RMB
    DrinkAbility UMETA(DisplayName = "DrinkAbility"),
    // 5 Q
    Throw UMETA(DisplayName = "Throw"),
    // 6 T
    Test UMETA(DisplayName = "Test"),
    // 7 SpaceBar
    FireProjectile UMETA(DisplayName = "FireProjectile")
};
