// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AlchemystEditorTarget : TargetRules
{
    public AlchemystEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        ExtraModuleNames.Add("Alchemyst");
    }
}