#include "ShaderIncludes.hlsli" 

TextureCube SurfaceTexture : register(t0); // "t" registers for textures
SamplerState BasicSampler : register(s0); // "s" registers for samplers


//make sure the input matches output of vertex shader
float4 main(VertexToPixelSky input) : SV_TARGET
{
	//get the cub map sample using the data from the input
	float3 cubeMapSample = SurfaceTexture.Sample(BasicSampler, input.sampleDir).rgb;

	return float4(cubeMapSample, 1);
}