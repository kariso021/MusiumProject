// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MusiumProject : ModuleRules
{
	public MusiumProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "Slate", "SlateCore", "UMG", "MediaAssets","Sockets", "Networking"});
	}
}
