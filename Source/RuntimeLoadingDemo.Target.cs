// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class RuntimeLoadingDemoTarget : TargetRules
{
	public RuntimeLoadingDemoTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_5;
		ExtraModuleNames.Add("RuntimeLoadingDemo");
        BuildEnvironment = TargetBuildEnvironment.Unique;
        GlobalDefinitions.Add("FORCE_ANSI_ALLOCATOR = 1");
    }
}
