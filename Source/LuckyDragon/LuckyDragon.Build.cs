// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LuckyDragon : ModuleRules
{
	public LuckyDragon(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "HTTP", "Json", "VaRest", "DeveloperSettings", "Sentry" });

		PrivateDependencyModuleNames.AddRange(new string[] {});
		// 只在编辑器构建中加载 UnrealEd 模块
		if (Target.Type == TargetRules.TargetType.Editor)
		{
			PrivateDependencyModuleNames.AddRange(new string[] { "Blutility" });
		}
		
		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
		
		PublicDefinitions.Add("DEBUG");
	}
}
