// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MCPServer : ModuleRules
{
	public MCPServer(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"HTTPServer",
			"Json",
			"JsonUtilities"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"UnrealEd",
			"LevelEditor",
			"AssetTools",
			"AssetRegistry"
		});
	}
}
