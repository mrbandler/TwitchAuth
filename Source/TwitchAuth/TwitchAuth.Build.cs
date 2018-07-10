// Copyright (c) 2018 fivefingergames.

using UnrealBuildTool;

public class TwitchAuth : ModuleRules
{
	public TwitchAuth(ReadOnlyTargetRules Target) : base(Target)
	{
        // OptimizeCode = CodeOptimization.Never;
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		// 4.15 Include What You Use
		// bEnforceIWYU = false;

		PublicIncludePaths.AddRange(
			new string[] {
				"TwitchAuth/Public",
                "WebBrowserWidget/Public"

				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"TwitchAuth/Private",
                "WebBrowserWidget/Private"

                // ... add other private include paths required here ...
            }
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "WebBrowserWidget"
            
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
                "UMG",
                "WebBrowser",
                "Http",
                "Json",
                "JsonUtilities"

				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
