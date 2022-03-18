#include "ShaderIncludes.hlsli" 
#include "Lighting.hlsli" 
#define NUM_LIGHTS 3
cbuffer ExternalData : register(b0)
{
	float roughness;
	float3 colorTint;
	float3 cameraPosition;
	float3 ambient;
	Light lights[NUM_LIGHTS];

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
	///////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////Ambient////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////
	//float3 lightTotal = (ambient * colorTint);
	float3 lightTotal = (0,0,0);
	////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	// Loop and handle all lights
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		Light light = lights[i];
		light.Direction = normalize(light.Direction);

		//run the right method for the right light
		switch (lights[i].Type)
		{		
			case LIGHT_TYPE_DIRECTIONAL:
				lightTotal += CreateDirectionalLight(lights[i], input.normal,roughness,colorTint,cameraPosition,input.worldPosition);
				break;

			case LIGHT_TYPE_POINT:
				break;
		}
	}
	///////////////////////////////////////////////////////////
	float3 finalPixelColor = lightTotal;
	return float4(finalPixelColor, 1);


}