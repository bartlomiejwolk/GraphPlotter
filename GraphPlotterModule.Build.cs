using UnrealBuildTool;

public class GraphPlotterModule : ModuleRules
{
    public GraphPlotterModule(ReadOnlyTargetRules Target) : base(Target)
    {
        MinFilesUsingPrecompiledHeaderOverride = 1;
        bFasterWithoutUnity = true;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });
        PrivateDependencyModuleNames.AddRange(new string[] { });

        PublicIncludePaths.AddRange(new string[] { "GraphPlotter/Public" });
        PrivateIncludePaths.AddRange(new string[] { "GraphPlotter/Private", "GraphPlotter/Utils" });

    }
}