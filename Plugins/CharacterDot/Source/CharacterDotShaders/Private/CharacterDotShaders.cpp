// Copyright Epic Games, Inc. All Rights Reserved.

#include "CharacterDotShaders.h"
#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FCharacterDotShadersModule"

void FCharacterDotShadersModule::StartupModule()
{
	FString PluginShaderDir = FPaths::Combine(IPluginManager::Get().FindPlugin(TEXT("CharacterDot"))->GetBaseDir(), TEXT("Shaders/Private"));
	AddShaderSourceDirectoryMapping(TEXT("/CharacterDotShaders"), PluginShaderDir);
}

void FCharacterDotShadersModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCharacterDotShadersModule, CharacterDotShaders)