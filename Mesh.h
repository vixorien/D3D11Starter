#pragma once

// Ben Coukos-Wiley
// 1/24/2023

#include <d3d11.h>
#include <wrl/client.h>
#include "Vertex.h"

class Mesh
{
public:
	/// <summary>
	/// Complete constructor for this mesh object
	/// </summary>
	/// <param name="vertices">An array of vertices used to draw this mesh</param>
	/// <param name="vertexCount">The total number of vertices in this mesh</param>
	/// <param name="indices">An array of indices used to draw this mesh</param>
	/// <param name="indexCount">The total number of indices in this mesh</param>
	/// <param name="device">A pointer to the Direct3D Device object</param>
	/// <param name="context">A pointer to the Direct3D Device Context object</param>
	Mesh(Vertex* vertices, int vertexCount, unsigned int* indices, int indexCount, Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
	~Mesh();

	/// <summary>
	/// Returns this mesh's vertex buffer
	/// </summary>
	/// <returns>This mesh's vertex buffer</returns>
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetVertexBuffer();
	/// <summary>
	/// Returns this mesh's index buffer
	/// </summary>
	/// <returns>This mesh's index buffer</returns>
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetIndexBuffer();
	/// <summary>
	/// Returns the number of indices in this mesh
	/// </summary>
	/// <returns>The number of indices in this mesh</returns>
	unsigned int GetIndexCount();
	/// <summary>
	/// Draws this mesh
	/// </summary>
	void Draw();

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

	Microsoft::WRL::ComPtr<ID3D11DeviceContext>	context;

	unsigned int indexCount;

};

