
#include "ShaderIncludes.hlsli" 
cbuffer ExternalData : register(b0)
{
	float3 colorTint;
	float roughness;
	float3 cameraPosition;
	float3 ambient; 
}


// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{
	return float4(colorTint * ambient ,0);
	//return float4(roughness.rrr, 1); // Replicates value x3 (this is temporary)
	//return float4(input.uv, 0, 1);
	//return float4(colorTint, 1);
}