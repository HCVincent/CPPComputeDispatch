#include "Shader_CharacterDot.h"

#include "GlobalShader.h"
#include "ShaderParameterStruct.h"
#include "RenderGraphBuilder.h"  
#include "RenderGraphUtils.h"

class FCharacterDotCS : public FGlobalShader
{
	DECLARE_EXPORTED_SHADER_TYPE(FCharacterDotCS, Global, CHARACTERDOTSHADERS_API);
	SHADER_USE_PARAMETER_STRUCT(FCharacterDotCS, FGlobalShader);

	BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
		SHADER_PARAMETER(uint32, Resolution)
		SHADER_PARAMETER(FVector2f, Scale)
		SHADER_PARAMETER(FVector2f, Origin)
		SHADER_PARAMETER(FVector2f, Location)
		SHADER_PARAMETER(float, Radius)
	SHADER_PARAMETER_RDG_TEXTURE_UAV(RWTexture2D<float>, OutTexture)
	END_SHADER_PARAMETER_STRUCT()

public:
	static constexpr uint32 NumThreadsX = 8;
	static constexpr uint32 NumThreadsY = 8;
	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
	{
		return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
	}

	static void ModifyCompilationEnvironment(const FGlobalShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment)
	{
		FGlobalShader::ModifyCompilationEnvironment(Parameters, OutEnvironment);
		OutEnvironment.SetDefine(TEXT("NUM_THREADS_X"), NumThreadsX);
		OutEnvironment.SetDefine(TEXT("NUM_THREADS_Y"), NumThreadsY);
	}
};

IMPLEMENT_GLOBAL_SHADER(FCharacterDotCS, "/CharacterDotShaders/CharacterDot.usf", "CharacterDot", SF_Compute);

void FCharacterDotShaderInterface::AddPass_RenderThread(FRDGBuilder& GraphBuilder, FGlobalShaderMap* InShaderMap, uint32 InResolution, const FVector2f& InScale, const FVector2f& Origin, const FVector2f& InLocation, float InRadius, FRDGTextureRef InTextureRef)
{
	ensure(IsInRenderingThread());

	RDG_EVENT_SCOPE(GraphBuilder, "CharacterDot");

	TShaderMapRef<FCharacterDotCS> ComputeShader(InShaderMap);

	FCharacterDotCS::FParameters* PassParameters = GraphBuilder.AllocParameters<FCharacterDotCS::FParameters>();

	PassParameters->Resolution = InResolution;
	PassParameters->Scale = InScale;
	PassParameters->Origin = Origin;
	PassParameters->Location = InLocation;
	PassParameters->Radius = InRadius;
	PassParameters->OutTexture = GraphBuilder.CreateUAV(InTextureRef);

	const FIntVector GroupCount(
		FMath::DivideAndRoundUp(InResolution, FCharacterDotCS::NumThreadsX),
		FMath::DivideAndRoundUp(InResolution, FCharacterDotCS::NumThreadsY),
		1
	);

	FComputeShaderUtils::AddPass(
		GraphBuilder,
		RDG_EVENT_NAME("CharacterDot"),
		ERDGPassFlags::Compute | ERDGPassFlags::NeverCull,
		ComputeShader,
		PassParameters,
		GroupCount
	);
}
