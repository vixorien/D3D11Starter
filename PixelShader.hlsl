#include "ShaderIncludes.hlsli" 

cbuffer ExternalData : register(b0)
{
	float3 colorTint;
	float roughness;
	float3 cameraPosition;
	float3 ambient;
	Light myDirectionLight;
}

//calculate the diffuse lighting with the normalized dir to light , and normals
float3 Diffuse(float3 normal, float3 dirToLight)
{
	return saturate(dot(normal, dirToLight));
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
	//make sure we normalize our normals coming from our vertex shader
	input.normal = normalize(input.normal);
	///////////////////////////////////////////////////////////////////////////////////////
	////////////////////////direction/////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////
	//make sure we negate the lights direction and normalize it for the diffuse function
	float3 dirToLight = normalize(-myDirectionLight.Direction);
	float3 diffuse = Diffuse(input.normal,dirToLight);
	float3 directionalLight = (diffuse * myDirectionLight.Color * colorTint);
	///////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////Ambient////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////
	float3 ambientLight = (ambient * colorTint);
	/////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////
	//for now directional than ambient 
	float3 finalPixelColor =  directionalLight + ambientLight;

	return float4(finalPixelColor, .5);


}