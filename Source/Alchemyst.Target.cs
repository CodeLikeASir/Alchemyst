// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AlchemystTarget : TargetRules
{
    public AlchemystTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        ExtraModuleNames.Add("Alchemyst");
    }
}