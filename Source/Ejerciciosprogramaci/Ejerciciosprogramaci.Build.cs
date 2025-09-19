// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Ejerciciosprogramaci : ModuleRules
{
	public Ejerciciosprogramaci(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"Ejerciciosprogramaci",
			"Ejerciciosprogramaci/Variant_Platforming",
			"Ejerciciosprogramaci/Variant_Platforming/Animation",
			"Ejerciciosprogramaci/Variant_Combat",
			"Ejerciciosprogramaci/Variant_Combat/AI",
			"Ejerciciosprogramaci/Variant_Combat/Animation",
			"Ejerciciosprogramaci/Variant_Combat/Gameplay",
			"Ejerciciosprogramaci/Variant_Combat/Interfaces",
			"Ejerciciosprogramaci/Variant_Combat/UI",
			"Ejerciciosprogramaci/Variant_SideScrolling",
			"Ejerciciosprogramaci/Variant_SideScrolling/AI",
			"Ejerciciosprogramaci/Variant_SideScrolling/Gameplay",
			"Ejerciciosprogramaci/Variant_SideScrolling/Interfaces",
			"Ejerciciosprogramaci/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
