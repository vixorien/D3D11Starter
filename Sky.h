#pragma once
#include "Camera.h"
#include <wrl/client.h> // Used for ComPtr - a smart pointer for COM objects
#include "Mesh.h"
#include "Material.h"
#include <memory>
#include "SimpleShader.h"
#include "DXCore.h"
#include "GameEntity.h"

class Sky {

	public:
		//constructor
		Sky(Microsoft::WRL::ComPtr<ID3D11Device> device,Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerStateFromGame, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> cubemapSRVFromGame, std::shared_ptr<Mesh> skyMeshFromGame, std::shared_ptr<SimpleVertexShader> vertexShaderFromGame, std::shared_ptr<SimplePixelShader> pixelShaderFromGame);
		void Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, std::shared_ptr<Camera> camera);

		//samplerstate for sky texture
		Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
		//srv for skymap texture
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> cubemapSRV;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthBuffer;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
		//shared ptr to hold our sky mesh
		std::shared_ptr<Mesh> skyMesh;
		std::shared_ptr<SimpleVertexShader> vertexShader;
		std::shared_ptr<SimplePixelShader> pixelShader;
		std::shared_ptr<Material> matSky;
		GameEntity* sky;
	private:
};