#pragma once
#include "DXCore.h"

class GameEntity
{
public:
	//constructor
	GameEntity(Mesh* mesh);
	//destructor
	~GameEntity();
	void Draw(Microsoft::WRL::ComPtr<ID3D11Buffer> vsConstantBuffer, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
	Mesh* GetMesh();

	Transform* getTransform();

private:
	//fields
	Transform entitysTransform;
	Mesh* entitysMesh;
};

