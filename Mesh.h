#pragma once

#include <wrl/client.h> // Used for ComPtr - a smart pointer for COM objects
#include <d3d11.h>
#include "DXCore.h"
#include "Vertex.h"

class Mesh
{

public:
	Mesh(
		Vertex* vertices,
		int vertexCount,
		unsigned int* indices,
		int indexCount,
		Microsoft::WRL::ComPtr<ID3D11Device> device,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
	~Mesh();
	
	// Functions
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetVertexBuffer();
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetIndexBuffer();
	int GetIndexCount();
	void Draw();

private:
	// Context
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>	context;

	// Counts
	int indexCount;

	// Buffers
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
};

