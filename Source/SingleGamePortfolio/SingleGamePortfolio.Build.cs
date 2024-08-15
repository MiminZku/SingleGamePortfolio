// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SingleGamePortfolio : ModuleRules
{
	public SingleGamePortfolio(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(new string[]
        {
            "SingleGamePortfolio"
        });

        PublicDependencyModuleNames.AddRange(new string[] 
		{ 
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"UMG",
			"AIModule",
			"NavigationSystem",
			"GameplayTasks",
			"GeometryCollectionEngine"
		});
	}
}
