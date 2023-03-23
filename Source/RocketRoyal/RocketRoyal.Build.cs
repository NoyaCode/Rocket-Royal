// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RocketRoyal : ModuleRules
{
	public RocketRoyal(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "Niagara" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}
