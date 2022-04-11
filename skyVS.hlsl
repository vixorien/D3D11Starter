#include "ShaderIncludes.hlsli" 
//describing layout of a chunk of memory
cbuffer ExternalData : register(b0)
{
	matrix view;
	matrix projection;
}

VertexToPixelSky main(VertexShaderInput input)
{
	// Set up output struct
	VertexToPixelSky output;

	//create a copy of view variable and remove all of its translations
	matrix viewNoTranslation = view;
	viewNoTranslation._14 = 0;
	viewNoTranslation._24 = 0;
	viewNoTranslation._34 = 0;

	//create a matrix to hold the view and projection so we can apply it 
	matrix vp = (mul(projection, viewNoTranslation));

	//now actually apply the matrix
	output.position = mul(vp, float4(input.localPosition, 1.0f));

	//set the depth of the output.pos to 1 by setting its z to its w because of what the rasterizer does
	output.position.z = output.position.w;

	//set sample direction equal to input position because INPP is just direction from origin
	output.sampleDir = input.localPosition;

	// Whatever we return will make its way through the pipeline to the
	// next programmable stage we're using (the pixel shader for now)
	return output;
}