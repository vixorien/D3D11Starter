#pragma once
#include "Mesh.h"
#include "Camera.h"
#include <wrl/client.h> // Used for ComPtr - a smart pointer for COM objects
#include <memory>
#include "Material.h"
class GameEntity
{
public:
	//constructor
	GameEntity(Mesh* mesh, std::shared_ptr<Material> mat);
	~GameEntity();
	/// ////////////////////////////////////////////////////////////////////////////
	void Draw( Microsoft::WRL::ComPtr<ID3D11DeviceContext> context,std::shared_ptr<Camera> camera);

	//getters and setters
	void SetMaterial(std::shared_ptr<Material> mat);
	std::shared_ptr<Material> GetMaterial();
	Mesh* GetMesh();
	Transform* getTransform();
private:
	//fields
	Transform entitysTransform;
	Mesh* entitysMesh;
	std::shared_ptr<Material> material;
};

