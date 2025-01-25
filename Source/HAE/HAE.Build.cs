// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class HAE : ModuleRules
{
	public HAE(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "HTTP", "Json", "JsonUtilities", "UMG", 
															"Sockets" , "Networking" , "ProceduralMeshComponent", "WebSockets", "SmartFactoryCommon" });

		// ���߿� �ٸ� ������Ʈ���� �÷����� ������ ��, ���� �ʿ�

		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
