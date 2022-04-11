#include "ShaderIncludes.hlsli" 
#include "Lighting.hlsli"
#define NUM_LIGHTS 5

cbuffer ExternalData : register(b0)
{
	float roughness;
	float3 colorTint;
	float3 cameraPosition;
	float3 ambient;
	Light lights[NUM_LIGHTS];

}

//for texture
Texture2D SurfaceTexture : register(t0); // "t" registers for textures
SamplerState BasicSampler : register(s0); // "s" registers for samplers
//normals of texture
Texture2D SurfaceTextureNormals : register(t1); 




float4 main(VertexToPixelNormalMapping input) : SV_TARGET
{
	//get our unpacked normals which we get by converting the color
	float3 unpackedNormal = SurfaceTextureNormals.Sample(BasicSampler, input.uv).rgb * 2 - 1;

	// Simplifications include not re-normalizing the same vector more than once!
	float3 N = normalize(input.normal); // Must be normalized here or before
	float3 T = normalize(input.tangent); // Must be normalized here or before
	T = normalize(T - N * dot(T, N)); // Gram-Schmidt assumes T&N are normalized!
	float3 B = cross(T, N);
	float3x3 TBN = float3x3(T, B, N);

	//finally produce our correct normals
	input.normal = mul(unpackedNormal, TBN); // Note multiplication order!

	//get our surfaceColor(texture)
	float3 surfaceColor = SurfaceTexture.Sample(BasicSampler, input.uv ).rgb;

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
	/*
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		Light light = lights[i];
		light.Direction = normalize(light.Direction);
		
		//run the right method for the right light
		switch (lights[i].Type)
		{
		case LIGHT_TYPE_DIRECTIONAL:
			lightTotal += CreateDirectionalLight(lights[i], input.normal, roughness, colorTint, cameraPosition, input.worldPosition);
			break;

		case LIGHT_TYPE_POINT:
			lightTotal += CreatePointLight(lights[i], input.normal, roughness, colorTint, cameraPosition, input.worldPosition);
			break;
		}
		
	}
	*/
	///////////////////////////////////////////////////////////
	float3 finalPixelColor = lightTotal;
	return float4(finalPixelColor, 1);


}