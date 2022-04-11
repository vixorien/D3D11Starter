#include "ShaderIncludes.hlsli" 
#include "Lighting.hlsli" 
#define NUM_LIGHTS 1

cbuffer ExternalData : register(b0)
{
	float roughness;
	float3 colorTint;
	float3 cameraPosition;
	float3 ambient;
	float scale;
	Light lights[NUM_LIGHTS];

}
Texture2D SurfaceTexture : register(t0); // "t" registers for textures
SamplerState BasicSampler : register(s0); // "s" registers for samplers


float4 main(VertexToPixel input) : SV_TARGET
{
	//make sure we normalize our normals coming from our vertex shader
	input.normal = normalize(input.normal);

	input.uv += scale;


	//get our surfaceColor(texture) and uncorrect gamma because if not we have already gamma corrected  image getting GCd again
	float3 surfaceColor = pow(SurfaceTexture.Sample(BasicSampler, input.uv*2).rgb,2.2);
	//multiply our surface color by our colorTint
	surfaceColor *= colorTint;

	///////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////Ambient////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////
	float3 lightTotal = (ambient * surfaceColor);
	//float3 lightTotal = (0,0,0);
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
				lightTotal += CreatePointLight(lights[i], input.normal, roughness, colorTint, cameraPosition, input.worldPosition);
				break;
		}
	}
	///////////////////////////////////////////////////////////
	float3 finalPixelColor = lightTotal;
	return float4(pow(finalPixelColor, 1.0f/2.2f), 1);


}