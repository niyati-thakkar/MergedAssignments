// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AssignmentsProject : ModuleRules
{
	public AssignmentsProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "Assignment_1_2", "Assignment_3_4", "EnhancedInput", "Slate", "SlateCore", "HeadMountedDisplay", "NavigationSystem", "AIModule", "Niagara", });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
