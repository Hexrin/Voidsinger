using UnrealBuildTool;

public class VoidsingerEditorTools : ModuleRules
{
    public VoidsingerEditorTools(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "UMG", 
            "EditorScriptingUtilities",
            "Voidsinger",
            "Blutility",
            "UnrealEd"
        });
        //if(Target.Configuration == UnrealTargetConfiguration.Development)
        //{
        //    PublicDependencyModuleNames.AddRange(new string[] {
                
        //    });
        //}
    }
}