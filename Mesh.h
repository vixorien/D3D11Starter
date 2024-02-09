#pragma once
#include <d3d11.h>;
#include "DXCore.h";
#include "Vertex.h";
#include <memory>;
#include "Input.h"
#include <d3dcompiler.h>

#include <wrl/client.h>

// std::shared_ptr<Mesh> triangle; // Declaration (probably in a header)
// triangle = std::make_shared<Mesh>(param1, param2, etc); // Initialization

class Mesh
{
public:
	Mesh(Vertex* vb, size_t numVerts, unsigned int* ib, size_t numIndices, Microsoft::WRL::ComPtr<ID3D11Device> device);
	~Mesh();

	void Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetVertexBuffer();
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetIndexBuffer();
	int GetIndexCount();
	int GetVertexCount();

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
	int indices;
	int vertices; 
};

