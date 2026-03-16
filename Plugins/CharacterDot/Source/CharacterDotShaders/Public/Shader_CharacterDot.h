

#pragma once

#include "CoreMinimal.h"

class CHARACTERDOTSHADERS_API FCharacterDotShaderInterface
{
public:
    static void AddPass_RenderThread(
        FRDGBuilder& GraphBuilder,
        FGlobalShaderMap* InShaderMap,
        uint32 InResolution,
        const FVector2f& InScale,
        const FVector2f& Origin,
        const FVector2f& InLocation,
        float InRadius,
        FRDGTextureRef InTextureRef
    );
};