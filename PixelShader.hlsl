#include "ShaderIncludes.hlsli" 

cbuffer ExternalData : register(b0)
{
	float3 colorTint;
	float roughness;
	float3 cameraPosition;
	float3 ambient;
	Light myDirectionLight;
	Light myDirectionLight2;
	Light myDirectionLight3;
}
//calculate the diffuse lighting with the normalized dir to light , and normals
float3 Diffuse(float3 normal, float3 dirToLight)
{
	return saturate(dot(normal, dirToLight));
}
float SpecularPhong(float3 normal, float3 incomingLightDir,float viewVector,float specExponent) {
	//calculate the perfect reflection of the incoming light
	float3 reflection = reflect(incomingLightDir, normal);

	//determine how close it is to out view vector 
	float spec = saturate(dot(reflection, viewVector));

	//raise to a power for quick fall off
	return pow(spec, specExponent);
}
float3 CreateDirectionalLight(Light light,float3 normalizedNormals, float3 viewVector)
{

	///////////////////////////////////////////////////////////////////////////////////////
	////////////////////////direction/////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////
	//make sure we negate the lights direction and normalize it for the diffuse function
	float3 dirToLight = normalize(-light.Direction);
	float3 diffuse = Diffuse(normalizedNormals, dirToLight);
	float3 diffuseLight = (diffuse * light.Color * colorTint);
	/////////////////////////////////////////////////////////////
	/////////////////////////specular phong///////////////////////////////////////////////////
	// 	   /////////////////////////////////////////////////////////////////////////////////////// 
	//declare lighttyper
	float specularPhongLight=0;
	//first lets get our specular component so we can craft our conditionals
	float specExponent = (1.0f - roughness) * MAX_SPECULAR_EXPONENT;
	//if shiny	
	if (specExponent > .05) 
	{
		//calculate incoming light direction

		specularPhongLight = (SpecularPhong(normalizedNormals, -dirToLight, viewVector, specExponent));
	}
	//if not
	else 
	{
		specularPhongLight = 0;
	}

	//return the final light
	float3 finalLight = diffuseLight + specularPhongLight;
	return finalLight;
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
	//these do not change
	//make sure we normalize our normals coming from our vertex shader
	input.normal = normalize(input.normal);
	float viewVector = normalize(cameraPosition - input.worldPosition);
	///////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////Ambient////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////
	float3 ambientLight = (ambient * colorTint);
	////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	float3 dirLight1 = CreateDirectionalLight(myDirectionLight, input.normal, viewVector);
	float3 dirLight2 = CreateDirectionalLight(myDirectionLight2, input.normal, viewVector);
	float3 dirLight3 = CreateDirectionalLight(myDirectionLight3, input.normal, viewVector);
	//////
	float3 finalPixelColor = dirLight1 + dirLight2 + dirLight3 + ambientLight;
	return float4(finalPixelColor, .5);


}