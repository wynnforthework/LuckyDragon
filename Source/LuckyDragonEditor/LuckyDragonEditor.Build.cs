using UnrealBuildTool;

public class LuckyDragonEditor : ModuleRules
{
	public LuckyDragonEditor(ReadOnlyTargetRules Target):base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] {"Core", "CoreUObject", "Engine", "LuckyDragon", "Slate", "SlateCore", "VaRest", "UMG","Json"});
		
		// PublicDependencyModuleNames.Add("LuckyDragon");
		
		PrivateDependencyModuleNames.AddRange(new string[] {"UnrealEd", "Blutility" });
	}
}
