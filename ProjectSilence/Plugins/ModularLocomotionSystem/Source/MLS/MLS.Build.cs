


using UnrealBuildTool;

public class MLS : ModuleRules
{
	public MLS(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new[]
			{"Core", "CoreUObject", "Engine", "InputCore", "NavigationSystem", "AIModule", "GameplayTasks","PhysicsCore", "Niagara", "EnhancedInput"
			});

		PrivateDependencyModuleNames.AddRange(new[] {"Slate", "SlateCore"});
	}
}