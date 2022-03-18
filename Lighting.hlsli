#ifndef __GGP_SHADER_INCLUDES_LIGHTING__ // Each .hlsli file needs a unique identifier! 
#define __GGP_SHADER_INCLUDES_LIGHTING__ 

//make sure we define our different light types
#define LIGHT_TYPE_DIRECTIONAL 0 
#define LIGHT_TYPE_POINT  1 
#define LIGHT_TYPE_SPOT  2
#define MAX_SPECULAR_EXPONENT 512.0f 

float Attenuate(Light light, float3 worldPos)
{
	float dist = distance(light.Position, worldPos);
	float att = saturate(1.0f - (dist * dist / (light.Range * light.Range)));
	return att * att;
}

//calculate the diffuse lighting with the normalized dir to light , and normals
float Diffuse(float3 normal, float3 dirToLight)
{
	return saturate(dot(normal, dirToLight));
}
float SpecularPhong(float3 normal, float3 dirToLight, float3 toCamera,float roughness) {
	// Calculate reflection vector
	float3 refl = reflect(-dirToLight, normal);

	// Compare reflection vector & view vector and raise to a power
	return pow(max(dot(toCamera, refl), 0), (1 - roughness) * MAX_SPECULAR_EXPONENT);

}
float3 CreateDirectionalLight(Light light, float3 normalizedNormals, float roughness, float3 colorTint, float3 cameraPosition, float3 worldPos)
{

	///////////////////////////////////////////////////////////////////////////////////////
	////////////////////////direction/////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////
	//make sure we negate the lights direction and normalize it for the diffuse function
	float3 dirToLight = normalize(-light.Direction);
	float diffuse = Diffuse(normalizedNormals, dirToLight);
	/////////////////////////////////////////////////////////////
	/////////////////////////specular phong///////////////////////////////////////////////////
	// 	   /////////////////////////////////////////////////////////////////////////////////////// 
	//declare lighttyper
	float specularPhongLight;
	//first lets get our specular component so we can craft our conditionals
	float specExponent = (1.0f - roughness) * MAX_SPECULAR_EXPONENT;
	//if shiny	
	if (specExponent > .05)
	{
		float3 viewVector = normalize(cameraPosition - worldPos);
		//calculate incoming light direction
		specularPhongLight = (SpecularPhong(normalizedNormals, dirToLight, viewVector, roughness));
	}
	//if not
	else
	{
		specularPhongLight = 0;
	}

	//return the final light
	float3 finalLight = (diffuse * colorTint + specularPhongLight) * light.Intensity * light.Color;
	return finalLight;
}
float3 CreatePointLight(Light light, float3 normalizedNormals, float roughness, float3 colorTint, float3 cameraPosition,float3 worldPosition )
{
	///////////////////////////////////////////////////////////////////////////////////////
	////////////////////////diffuse/////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////
	//get the direction to the light
	float3 dirToLight = normalize(light.Position - worldPosition);
	//calculate our diffuse
	float diffuse = Diffuse(normalizedNormals, dirToLight);
	/////////////////////////////////////////////////////////////
	/////////////////////////specular phong///////////////////////////////////////////////////
	// 	   /////////////////////////////////////////////////////////////////////////////////////// 
	//declare lighttyper
	float specularPhongLight;
	//first lets get our specular component so we can craft our conditionals
	float specExponent = (1.0f - roughness) * MAX_SPECULAR_EXPONENT;
	//if shiny	
	if (specExponent > .05)
	{
		float3 viewVector = normalize(cameraPosition - worldPosition);
		//calculate incoming light direction
		specularPhongLight = (SpecularPhong(normalizedNormals, dirToLight, viewVector, roughness));
	}
	//if not
	else
	{
		specularPhongLight = 0;
	}
	/////////////////////////////////////////////////////////////
	/////////////////////////atten///////////////////////////////////////////////////
	// 	   /////////////////////////////////////////////////////////////////////////////////////// 
	float attenuate = Attenuate(light, worldPosition);
	
	
	return (diffuse * colorTint + specularPhongLight) * attenuate * light.Intensity * light.Color;
}
#endif