// Copyright Epic Games, Inc. All Rights Reserved.

//#define CRASHSIGHT_HARMONY

using UnrealBuildTool;
using System.IO;
using System;
using System.Reflection;
public class CrashSight : ModuleRules
{
    public CrashSight(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

#if UE_4_20_OR_LATER
        // ue 4.20 or later do not need PublicIncludePaths
#else
        PublicIncludePaths.AddRange(
            new string[] {
                "CrashSight/Public",
            }
            );
#endif

        PrivateIncludePaths.AddRange(
            new string[] {
                "CrashSight/Private"
            }
            );


        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                // ... add other public dependencies that you statically link with here ...
            }
            );


        if (Target.Platform == UnrealTargetPlatform.IOS ||
            Target.Platform == UnrealTargetPlatform.Mac)
        {
            PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                // ... add private dependencies that you statically link with here ...
            }
            );
        }
        else
        {
            PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                // ... add private dependencies that you statically link with here ...

            }
            );
        }


        if (Target.Type == TargetRules.TargetType.Editor)
        {

            DynamicallyLoadedModuleNames.AddRange(
                        new string[] {
                    "Settings",
                    }
                    );

            PrivateIncludePathModuleNames.AddRange(
                new string[] {
                    "Settings",
            }
            );
        }


        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            //Add Android
            PrivateDependencyModuleNames.AddRange(new string[] { "Launch" });

#if UE_4_24_OR_LATER
            PublicSystemLibraryPaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "../CrashSightLib/Android/libs/armeabi-v7a")));
            PublicSystemLibraryPaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "../CrashSightLib/Android/libs/arm64-v8a")));
            PublicSystemLibraryPaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "../CrashSightLib/Android/libs/x86")));
            PublicSystemLibraryPaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "../CrashSightLib/Android/libs/x86_64")));
#else
            PublicLibraryPaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "../CrashSightLib/Android/libs/armeabi-v7a")));
            PublicLibraryPaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "../CrashSightLib/Android/libs/arm64-v8a")));
            PublicLibraryPaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "../CrashSightLib/Android/libs/x86")));
            PublicLibraryPaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "../CrashSightLib/Android/libs/x86_64")));
#endif

            System.Console.WriteLine("--------------Add Android CrashSight");
            PublicAdditionalLibraries.Add("CrashSight");

            string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
#if UE_4_20_OR_LATER
            AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(PluginPath, "CrashSight_APL.xml"));
#else
            AdditionalPropertiesForReceipt.Add(new ReceiptProperty("AndroidPlugin", Path.Combine(PluginPath, "CrashSight_APL.xml")));
#endif
            System.Console.WriteLine("CrashSight APL Path = " + Path.Combine(PluginPath, "CrashSight_APL.xml"));

        }
        else if (Target.Platform == UnrealTargetPlatform.IOS)
        {
#if UE_4_24_OR_LATER
            PublicSystemLibraries.AddRange(
                new string[] {
                 "z","c++","z.1.1.3","sqlite3","xml2","resolv"
            });
#else
            PublicAdditionalLibraries.AddRange(
                new string[] {
                "z","c++","z.1.1.3"
            });
#endif
            // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
            PublicFrameworks.AddRange(
                new string[] {
                    "Foundation",
                    "Security"
                                    });

            PublicWeakFrameworks.AddRange(
            new string[] {
                    "MetricKit",
                    "OSLog"
                });

            PublicAdditionalFrameworks.Add(
#if UE_5_2_OR_LATER
                new Framework("CrashSight", "../CrashSightLib/iOS/CrashSight.embeddedframework.zip", null, true)
#elif UE_4_22_OR_LATER
                new Framework("CrashSight", "../CrashSightLib/iOS/CrashSight.embeddedframework.zip", "Frameworks")
#else
                new UEBuildFramework("CrashSight", "../CrashSightLib/iOS/CrashSight.embeddedframework.zip", "Frameworks")
#endif
            );

            PublicAdditionalFrameworks.Add(
#if UE_5_2_OR_LATER
                new Framework("CrashSightAdapter", "../CrashSightLib/iOS/CrashSightAdapter.embeddedframework.zip", null, true)
#elif UE_4_22_OR_LATER
                new Framework("CrashSightAdapter", "../CrashSightLib/iOS/CrashSightAdapter.embeddedframework.zip", "Frameworks")
#else
                new UEBuildFramework("CrashSightAdapter", "../CrashSightLib/iOS/CrashSightAdapter.embeddedframework.zip", "Frameworks")
#endif
            );

            PublicAdditionalFrameworks.Add(
#if UE_5_2_OR_LATER
                new Framework("CrashSightCore", "../CrashSightLib/iOS/CrashSightCore.embeddedframework.zip", null, true)
#elif UE_4_22_OR_LATER
                new Framework("CrashSightCore", "../CrashSightLib/iOS/CrashSightCore.embeddedframework.zip", "Frameworks")
#else
                new UEBuildFramework("CrashSightCore", "../CrashSightLib/iOS/CrashSightCore.embeddedframework.zip", "Frameworks")
#endif
            );

            PublicAdditionalFrameworks.Add(
#if UE_5_2_OR_LATER
                new Framework("CrashSightPlugin", "../CrashSightLib/iOS/CrashSightPlugin.embeddedframework.zip", null, true)
#elif UE_4_22_OR_LATER
                new Framework("CrashSightPlugin", "../CrashSightLib/iOS/CrashSightPlugin.embeddedframework.zip", "Frameworks")
#else
                new UEBuildFramework("CrashSightPlugin", "../CrashSightLib/iOS/CrashSightPlugin.embeddedframework.zip", "Frameworks")
#endif
            );

        }
        else if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            PublicDelayLoadDLLs.Add("CrashSight64.dll");

            // Ensure that the DLL is staged along with the executable
#if UE_4_22_OR_LATER
             RuntimeDependencies.Add("$(TargetOutputDir)/...", "$(PluginDir)/Source/CrashSightLib/Win/X86_64/...");
#else
            // new UEBuildFramework("CrashSightPlugin", "../CrashSightLib/iOS/CrashSightPlugin.embeddedframework.zip")
#endif
        }
        else if (Target.Platform == UnrealTargetPlatform.Linux)
        {
            System.Console.WriteLine("--------------Add Linux CrashSight");
            string CrashSightThirdPartyDir = Path.GetFullPath(Path.Combine(ModuleDirectory, "../CrashSightLib"));
            string CrashSightLibPath = Path.Combine(CrashSightThirdPartyDir, "Linux");
            System.Console.WriteLine("--------------Linux CrashSightLibPath = " + CrashSightLibPath);
#if UE_4_24_OR_LATER
            PublicSystemLibraryPaths.Add(Path.GetFullPath(CrashSightLibPath));
#else
            PublicLibraryPaths.Add(Path.GetFullPath(CrashSightLibPath));
#endif
            PublicAdditionalLibraries.Add(Path.Combine(CrashSightLibPath, "libCrashSight.so"));
#if UE_4_22_OR_LATER
            RuntimeDependencies.Add("$(TargetOutputDir)/libCrashSight.so", Path.Combine(CrashSightLibPath, "libCrashSight.so"));
#endif
        }
        else if (IsSameUnrealTargetPlatformCrashSight("XSX", Target.Platform))
        {
#if UE_4_20_OR_LATER
             PublicDefinitions.Add("CRASHSIGHT_XSX=1");
#else
            Definitions.Add("CRASHSIGHT_XSX=1");
#endif
            string CrashSightThirdPartyDir = Path.GetFullPath(Path.Combine(ModuleDirectory, "../CrashSightLib"));
            string CrashSightLibPath = Path.Combine(CrashSightThirdPartyDir, "XSX\\");
            System.Console.WriteLine("--------------XSX CrashSightLibPath = " + CrashSightLibPath);
#if UE_4_24_OR_LATER
            PublicSystemLibraryPaths.Add(CrashSightLibPath);
#else
            PublicLibraryPaths.Add(CrashSightLibPath);
#endif
#if UE_4_22_OR_LATER
            RuntimeDependencies.Add("$(TargetOutputDir)/CrashSightXbox.dll", Path.Combine(CrashSightLibPath, "CrashSightXbox.dll"));
#endif
        }
        else if (IsSameUnrealTargetPlatformCrashSight("PS4", Target.Platform))
        {
#if UE_4_20_OR_LATER
             PublicDefinitions.Add("CRASHSIGHT_PS4=1");
#else
            Definitions.Add("CRASHSIGHT_PS4=1");
#endif
#if UE_4_24_OR_LATER
            PublicSystemLibraries.AddRange(new string[]
            {
                 "SceNetCtl_stub_weak",
                 "SceNet_stub_weak",
                 "SceHttp_stub_weak",
                 "SceSsl_stub_weak"
            });
#else
            PublicAdditionalLibraries.AddRange(new string[]
            {
                 "SceNetCtl_stub_weak",
                 "SceNet_stub_weak",
                 "SceHttp_stub_weak",
                 "SceSsl_stub_weak"
            });
#endif
            string CrashSightThirdPartyDir = Path.GetFullPath(Path.Combine(ModuleDirectory, "../CrashSightLib"));
            string CrashSightLibPath = Path.Combine(CrashSightThirdPartyDir, "PS4/");
            System.Console.WriteLine("--------------PS4 CrashSightLibPath = " + CrashSightLibPath);

#if UE_4_24_OR_LATER
            PublicSystemLibraryPaths.Add(CrashSightLibPath);
#else
            PublicLibraryPaths.Add(CrashSightLibPath);
#endif
            PublicAdditionalLibraries.Add(Path.Combine(CrashSightLibPath, "libcs_stub.a"));

#if UE_4_22_OR_LATER
            RuntimeDependencies.Add(Path.Combine(CrashSightLibPath, "libcs.prx"));
#endif

            PublicDelayLoadDLLs.Add(Path.Combine(CrashSightLibPath, "libcs.prx"));
        }
        else if (IsSameUnrealTargetPlatformCrashSight("PS5", Target.Platform))
        {
#if UE_4_20_OR_LATER
             PublicDefinitions.Add("CRASHSIGHT_PS5=1");
#else
            Definitions.Add("CRASHSIGHT_PS5=1");
#endif
#if UE_4_24_OR_LATER
            PublicSystemLibraries.AddRange(new string[]
            {
                 "SceNetCtl_stub_weak",
                 "SceNet_stub_weak",
                 "SceHttp_stub_weak",
                 "SceSsl_stub_weak"
            });
#else
            PublicAdditionalLibraries.AddRange(new string[]
            {
                 "SceNetCtl_stub_weak",
                 "SceNet_stub_weak",
                 "SceHttp_stub_weak",
                 "SceSsl_stub_weak"
            });
#endif
            string CrashSightThirdPartyDir = Path.GetFullPath(Path.Combine(ModuleDirectory, "../CrashSightLib"));
            string CrashSightLibPath = Path.Combine(CrashSightThirdPartyDir, "PS5/");
            System.Console.WriteLine("--------------PS5 CrashSightLibPath = " + CrashSightLibPath);

#if UE_4_24_OR_LATER
            PublicSystemLibraryPaths.Add(CrashSightLibPath);
#else
            PublicLibraryPaths.Add(CrashSightLibPath);
#endif
            PublicAdditionalLibraries.Add(Path.Combine(CrashSightLibPath, "libcs_stub.a"));

#if UE_4_22_OR_LATER
            RuntimeDependencies.Add(Path.Combine(CrashSightLibPath, "libcs.prx"));
#endif

            PublicDelayLoadDLLs.Add(Path.Combine(CrashSightLibPath, "libcs.prx"));
        }
        else if (IsSameUnrealTargetPlatformCrashSight("Switch", Target.Platform))
        {
#if UE_4_20_OR_LATER
             PublicDefinitions.Add("CRASHSIGHT_SWITCH=1");
#else
            Definitions.Add("CRASHSIGHT_SWITCH=1");
#endif
            string CrashSightThirdPartyDir = Path.GetFullPath(Path.Combine(ModuleDirectory, "../CrashSightLib"));
            string CrashSightLibPath = Path.Combine(CrashSightThirdPartyDir, "Switch/");
            System.Console.WriteLine("--------------Switch CrashSightLibPath = " + CrashSightLibPath);
            PublicAdditionalLibraries.Add(Path.Combine(CrashSightLibPath, "libcs.a"));
        }
#if CRASHSIGHT_HARMONY
        else if (IsSameUnrealTargetPlatformCrashSight("OpenHarmony", Target.Platform))
        {
            PrivateDependencyModuleNames.AddRange(new string[] { "Launch", "aki_jsbind" });
#if UE_4_20_OR_LATER
			PublicDefinitions.Add("CRASHSIGHT_OHOS");
#else
            Definitions.Add("CRASHSIGHT_OHOS");
#endif

            string PluginPath = Path.GetFullPath(Path.Combine(ModuleDirectory, "../CrashSightLib"));
            System.Console.WriteLine("--------------OpenHarmony CrashSight libs = " + PluginPath);
            PublicAdditionalHars.Add(Path.Combine(PluginPath, "OpenHarmony/crashsight.har"));
            string crashSightRootLibDir = Path.GetFullPath(Path.Combine(ModuleDirectory, "OpenHarmony/libs"));
            string oplPath = Path.Combine(PluginPath, "OpenHarmony/CrashSight_OPL.xml");
#if UE_4_20_OR_LATER
			AdditionalPropertiesForReceipt.Add("OpenHarmonyPlugin", oplPath);
#else 
            AdditionalPropertiesForReceipt.Add(new ReceiptProperty("OpenHarmonyPlugin", oplPath));
#endif
            System.Console.WriteLine("CrashSight_OPL ROOTPATH : " + crashSightRootLibDir);
            PublicAdditionalLibraries.Add("crashsight");
        }
#endif
    }

    private bool IsSameUnrealTargetPlatformCrashSight(String platformName, UnrealTargetPlatform currentPlatform)
    {
        FieldInfo platformField = typeof(UnrealTargetPlatform).GetField(platformName);
        if (platformField != null)
        {
            UnrealTargetPlatform platform = (UnrealTargetPlatform)platformField.GetValue(null);
            System.Console.WriteLine("Find platform: " + platformName);
            return platform == currentPlatform;
        }
        else
        {
            System.Console.WriteLine("Unable to get platform: " + platformName);
            return false;
        }
    }
}
