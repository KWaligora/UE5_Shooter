using UnrealBuildTool;

public class MShooter : ModuleRules
{
	public MShooter(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "OnlineSubsystem", "OnlineSubsystemSteam", "UMG" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}
