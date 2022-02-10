#pragma once
#include "DXCore.h"
#include "Camera.h"

class GameEntity
{
public:
	//constructor
	GameEntity(Mesh* mesh);
	//destructor
	~GameEntity();
	void Draw(Microsoft::WRL::ComPtr<ID3D11Buffer> vsConstantBuffer, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context,std::shared_ptr<Camera> camera);
	Mesh* GetMesh();

	Transform* getTransform();

private:
	//fields
	Transform entitysTransform;
	Mesh* entitysMesh;
};

