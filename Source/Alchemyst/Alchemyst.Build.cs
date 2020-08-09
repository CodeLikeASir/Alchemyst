// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Alchemyst : ModuleRules
{
	public Alchemyst(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule" });
    }
}
