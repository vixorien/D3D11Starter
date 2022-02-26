#pragma once
#include <memory>
#include <vector>
#include "SimpleShader.h"
#include "DXCore.h"
using namespace DirectX;
class Material
{
public:
	Material(std::shared_ptr<SimpleVertexShader>vertexShader, std::shared_ptr<SimplePixelShader>pixelShader, XMFLOAT3 colorTint);
	~Material();

	//getters and setters
	std::shared_ptr<SimplePixelShader> GetPixelShader();
	std::shared_ptr<SimpleVertexShader> GetVertexShader();
	XMFLOAT3 GetColorTint();

	void SetPixelShader(std::shared_ptr<SimplePixelShader> pixelShader);
	void SetVertexShader(std::shared_ptr<SimpleVertexShader> vertexShader);
	void SetColorTint(XMFLOAT3 colorTint);

private:
	//shared ptrs for our shader
	std::shared_ptr<SimplePixelShader> pixelShader;
	std::shared_ptr<SimpleVertexShader> vertexShader;

	XMFLOAT3 colorTint;

};