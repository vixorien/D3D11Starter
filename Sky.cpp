#include "Sky.h"

Sky::Sky(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerStateFromGame, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> cubemapSRVFromGame, std::shared_ptr<Mesh> skyMeshFromGame, std::shared_ptr<SimpleVertexShader> vertexShaderFromGame, std::shared_ptr<SimplePixelShader> pixelShaderFromGame)
{
	//setting memeber variables 
	samplerState = samplerStateFromGame;
	cubemapSRV = cubemapSRVFromGame;
	skyMesh = skyMeshFromGame;
	pixelShader = pixelShaderFromGame;
	vertexShader = vertexShaderFromGame;


	//handling ras state
	D3D11_RASTERIZER_DESC rasDesc = {};
	rasDesc.FillMode = D3D11_FILL_SOLID;
	rasDesc.CullMode = D3D11_CULL_FRONT;
	device->CreateRasterizerState(&rasDesc, rasterizerState.GetAddressOf());
	//handling depth strncil
	D3D11_DEPTH_STENCIL_DESC DSState = {};
	DSState.DepthEnable = true;
	DSState.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	device->CreateDepthStencilState(&DSState, depthBuffer.GetAddressOf());
}

void Sky::Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, std::shared_ptr<Camera> camera)
{
	//set rasterizing settings
	context->RSSetState(rasterizerState.Get());
	//set depth settings
	context->OMSetDepthStencilState(depthBuffer.Get(), 0);

	//set our shaders
	vertexShader->SetShader();
	pixelShader->SetShader();
	//set data in vertex shader cbuffer
	vertexShader->SetMatrix4x4("view", camera->GetViewMatrix());             // names in the  
	vertexShader->SetMatrix4x4("projection", camera->GetProjectionMatrix()); // shader’s cbuffer!
	vertexShader->CopyAllBufferData();
	//set textures in pixel shader
	pixelShader->SetShaderResourceView("SurfaceTexture", cubemapSRV);
	pixelShader->SetSamplerState("BasicSampler",samplerState);

	//draw our mesh to screen
	skyMesh->Draw();

	context->RSSetState(0);
	context->OMSetDepthStencilState(0, 0);
}
