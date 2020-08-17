// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Alchemyst : ModuleRules
{
    public Alchemyst(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule", "UMG",
            "Niagara"
        });
        PrivateDependencyModuleNames.AddRange(new[] {"GameplayAbilities", "GameplayTags", "GameplayTasks"});
    }
}