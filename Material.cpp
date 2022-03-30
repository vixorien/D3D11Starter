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

void Material::AddTextureSRV(std::string textureSRVName, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> SRV)
{
	//insert the key value pairs into out T UO Map
	textureSRVs.insert({ textureSRVName,SRV});
}

void Material::AddSampler(std::string samplerName, Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler)
{
	samplers.insert({ samplerName,sampler});
}

void Material::BindTexturesAndSamplers()
{
	//t.first is the name, t.second is the value(the actual object)
	for (auto& t : textureSRVs) { pixelShader->SetShaderResourceView(t.first.c_str(), t.second); }
	for (auto& s : samplers) { pixelShader->SetSamplerState(s.first.c_str(), s.second); }
}
