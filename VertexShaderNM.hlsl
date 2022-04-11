#include "ShaderIncludes.hlsli" 
//describing layout of a chunk of memory
cbuffer ExternalData : register(b0)
{
	matrix worldMatrix;
	matrix view;
	matrix projection;
	matrix invTransposeWorldMatrix;
}

VertexToPixelNormalMapping main(VertexShaderInput input)
{
	// Set up output struct
	VertexToPixelNormalMapping output;

	// Here we're essentially passing the input position directly through to the next
	// stage (rasterizer), though it needs to be a 4-component vector now.  
	// - To be considered within the bounds of the screen, the X and Y components 
	//   must be between -1 and 1.  
	// - The Z component must be between 0 and 1.  
	// - Each of these components is then automatically divided by the W component, 
	//   which we're leaving at 1.0 for now (this is more useful when dealing with 
	//   a perspective projection matrix, which we'll get to in the future).
	matrix wvp = mul((mul(projection, view)), worldMatrix);

	output.screenPosition = mul(wvp, float4(input.localPosition, 1.0f));
	output.normal = mul((float3x3)invTransposeWorldMatrix, input.normal);
	output.tangent = mul((float3x3)invTransposeWorldMatrix, input.tangent);
	output.worldPosition = mul(worldMatrix, float4(input.localPosition, 1)).xyz;
	output.uv = input.uv;


	// Whatever we return will make its way through the pipeline to the
	// next programmable stage we're using (the pixel shader for now)
	return output;
}