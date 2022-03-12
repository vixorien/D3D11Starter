#include "Material.h"
//set everything up
Material::Material(std::shared_ptr<SimpleVertexShader> vertexShader, std::shared_ptr<SimplePixelShader> pixelShader, XMFLOAT3 colorTint,float roughness)
{
	SetColorTint(colorTint);
	SetVertexShader(vertexShader);
	SetPixelShader(pixelShader);
	SetRoughness(roughness);
}
//all shared pointers so not neccessary
Material::~Material()
{

}

std::shared_ptr<SimplePixelShader> Material::GetPixelShader()
{
	return pixelShader;
}

std::shared_ptr<SimpleVertexShader> Material::GetVertexShader()
{
	return vertexShader;
}

XMFLOAT3 Material::GetColorTint()
{
	return colorTint;
}

float Material::GetRoughness()
{
	return roughness;
}

void Material::SetPixelShader(std::shared_ptr<SimplePixelShader> pixelShader)
{
	this->pixelShader = pixelShader;
}

void Material::SetVertexShader(std::shared_ptr<SimpleVertexShader> vertexShader)
{
	this->vertexShader = vertexShader;
}

void Material::SetColorTint(XMFLOAT3 colorTint)
{
	this->colorTint = colorTint;
}

void Material::SetRoughness(float roughnessParam)
{
	this->roughness = roughnessParam;
}
