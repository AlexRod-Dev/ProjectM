// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectMTarget : TargetRules
{
	public ProjectMTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		bUsesSteam = true;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange(new[] { "ProjectM" });

	}
}