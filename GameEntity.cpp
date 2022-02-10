#include "GameEntity.h"
#include "BufferStructs.h"
GameEntity::GameEntity(Mesh* mesh)
{
    entitysMesh = mesh;
}

GameEntity::~GameEntity()
{

}
//going to do option two because option 1 doesnt make sense to me
//passing in our constantbuffer and context so that we draw the idnividual entity we want
void GameEntity::Draw(Microsoft::WRL::ComPtr<ID3D11Buffer> vsConstantBuffer, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, std::shared_ptr<Camera> camera)
{
    //creating a place to store out jimmy jawns
    VertexShaderExternalData vsData;
    //passing over our worl matrix
	// 
	//add our color tint here
	///////////////////////////////////////////////////////////////




    vsData.worldMatrix = entitysTransform.BuildMatrix();
	vsData.viewMatrix = camera->GetViewMatrix();
	vsData.projectionMatrix = camera->GetprojectionMatrix();

	//copy over data and map it so lock it off from our c++ code
	D3D11_MAPPED_SUBRESOURCE mapBuffer = {};
	context->Map(vsConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapBuffer);
	//copy out vsdata over to our mapbuffer data
	memcpy(mapBuffer.pData, &vsData, sizeof(vsData));
	//unmap the biffer so c++ can play
	context->Unmap(vsConstantBuffer.Get(), 0);
	// Draw the object
	entitysMesh->Draw();
}

Mesh* GameEntity::GetMesh()
{
    return entitysMesh;
}

Transform* GameEntity::getTransform()
{
    return &entitysTransform;
}
