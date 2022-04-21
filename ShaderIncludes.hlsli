#ifndef __GGP_SHADER_INCLUDES__ // Each .hlsli file needs a unique identifier! 
#define __GGP_SHADER_INCLUDES__ 
// Struct representing a single vertex worth of data
// - This should match the vertex definition in our C++ code
// - By "match", I mean the size, order and number of members
// - The name of the struct itself is unimportant, but should be descriptive
// - Each variable must have a semantic, which defines its usage
struct VertexShaderInput
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float3 localPosition : POSITION; // XYZ position
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;


};
struct VertexToPixelSky
{
	float4 position : SV_POSITION;
	float3 sampleDir : DIRECTION;
};
// Struct representing the data we're sending down the pipeline
// - Should match our pixel shader's input (hence the name: Vertex to Pixel)
// - At a minimum, we need a piece of data defined tagged as SV_POSITION
// - The name of the struct itself is unimportant, but should be descriptive
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 screenPosition : SV_POSITION; // XYZW position (System Value Position)
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
	float3 worldPosition : POSITION;
};
struct VertexToPixelNormalMapping
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 screenPosition : SV_POSITION; // XYZW position (System Value Position)
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 worldPosition : POSITION;


};
struct Light
{
	int Type; // Which kind of light?  0, 1 or 2 (see above) 
	float3 Direction; // Directional and Spot lights need a direction 
	float Range; // Point and Spot lights have a max range for attenuation 
	float3 Position; // Point and Spot lights have a position in space 
	float Intensity; // All lights need an intensity 
	float3 Color; // All lights need a color 
	float SpotFalloff; // Spot lights need a value to define their “cone” size 
	float3 Padding;

};
#endif