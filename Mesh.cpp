#include "Mesh.h"
#include <iostream>

// ------------------------------------------------
// Constructor - Initialize the Mesh
//  - Create or allocate all the passed in variables
// ------------------------------------------------
Mesh::Mesh(
	Vertex* vertices, 
	int vertexCount, 
	unsigned int* indices, 
	int _indexCount, 
	Microsoft::WRL::ComPtr<ID3D11Device> device,
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> _context)
	:
	indexCount(_indexCount),
	context(_context)
{
	// Create the vertex buffer
	{
		// Describe the vertex buffer
		D3D11_BUFFER_DESC vbd = {};
		vbd.Usage = D3D11_USAGE_IMMUTABLE;	// Will NEVER change
		vbd.ByteWidth = sizeof(Vertex) * vertexCount;       // number of vertices in the buffer
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // Tells Direct3D this is a vertex buffer
		vbd.CPUAccessFlags = 0;	// Note: We cannot access the data from C++ (this is good)
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;

		// Create the proper struct to hold the vertex data
		D3D11_SUBRESOURCE_DATA res_VertexData = {};
		res_VertexData.pSysMem = vertices; // pSysMem = Pointer to System Memory

		// Actually create the vertex buffer on the GPU
		std::cout << device->CreateBuffer(&vbd, &res_VertexData, vertexBuffer.GetAddressOf()) <<std::endl;
	}
	
	// Create the index buffer
	{
		// Describe the index buffer
		D3D11_BUFFER_DESC ibd = {};
		ibd.Usage = D3D11_USAGE_IMMUTABLE;	// Will NEVER change
		ibd.ByteWidth = sizeof(unsigned int) * _indexCount;	// number of indices in the buffer
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;	// Tells Direct3D this is an index buffer
		ibd.CPUAccessFlags = 0;	// Note: We cannot access the data from C++ (this is good)
		ibd.MiscFlags = 0;
		ibd.StructureByteStride = 0;

		// Create the proper struct to hold the index data
		D3D11_SUBRESOURCE_DATA res_IndexData = {};
		res_IndexData.pSysMem = vertices; // pSysMem = Pointer to System Memory

		// Actually create the vertex buffer on the GPU
		std::cout << device->CreateBuffer(&ibd, &res_IndexData, indexBuffer.GetAddressOf()) << std::endl;
	}
}

// --------------------------------------------------------
// Destructor - Clean up anything created by the Mesh
//  - Delete all objects manually created within this class
//  - Release() all Direct3D objects created within this class
// --------------------------------------------------------
Mesh::~Mesh()
{
	// Call delete or delete[] on any objects or arrays 
	//  - Unnecessary if using smart pointers

	// Call Release() on any Direct3D objects made within this class
	// - Unnecessary for D3D objects stored in ComPtrs
}

// ----------------------------------
// Getter funciton for Vertex buffer
// ----------------------------------
Microsoft::WRL::ComPtr<ID3D11Buffer> Mesh::GetVertexBuffer()
{
	return vertexBuffer;
}

// ---------------------------------
// Getter function for Index buffer
// ---------------------------------
Microsoft::WRL::ComPtr<ID3D11Buffer> Mesh::GetIndexBuffer()
{
	return indexBuffer;
}

// ---------------------------------
// Getter function for index count
// ---------------------------------
int Mesh::GetIndexCount()
{
	return indexCount;
}

// --------------
// Draw function
// --------------
void Mesh::Draw()
{
	// Initial values
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	{
		// Set buffers in the input assembler (IA) stage
		context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
		context->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		// Tell Direct3D to draw
		context->DrawIndexed(
			indexCount,     // The number of indices to use
			0,     // Offset to the first index we want to use
			0);    // Offset to add to each index when looking up vertices
	}
}
