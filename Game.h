#pragma once
#include "Mesh.h"
#include "Camera.h"
#include "GameEntity.h"
#include "DXCore.h"
#include <memory>
#include <vector>
#include "SimpleShader.h"
#include "Material.h"
#include "Lights.h"
#include "Sky.h"
class Game 
	: public DXCore
{

public:
	Game(HINSTANCE hInstance);
	~Game();

	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void initImGui();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);
	//creating our 3 meshes for our shapes
	
	

private:
	std::vector<GameEntity*> listOfEntitys;
	//entity
	//shapes and meshes
	std::shared_ptr<Mesh> shapeOne;
	std::shared_ptr<Mesh> shapeTwo;
	std::shared_ptr<Mesh> shapeThree;
	std::shared_ptr<Mesh> shapeFour;
	std::shared_ptr<Mesh> shapeFive;
	std::shared_ptr<Mesh> shapeSix;
	//transform
	Transform transform;
	//camera
	std::shared_ptr<Camera> camera;
	//shaders
	std::shared_ptr<SimplePixelShader> pixelShader;
	std::shared_ptr<SimplePixelShader> pixelShader2;
	std::shared_ptr<SimpleVertexShader> vertexShader;

	std::shared_ptr<SimpleVertexShader> vertexShaderNM;
	std::shared_ptr<SimplePixelShader> pixelShaderNM;

	std::shared_ptr<SimpleVertexShader> vertexShaderSky;
	std::shared_ptr<SimplePixelShader> pixelShaderSky;
	//materials
	std::shared_ptr<Material> mat1;
	std::shared_ptr<Material> mat2;
	std::shared_ptr<Material> mat3;
	std::shared_ptr<Material> mat4;
	std::shared_ptr<Material> mat5;
	std::shared_ptr<Material> matSky;
	//lights and light data
	XMFLOAT3 ambientColor;
	std::vector<Light> lights;
	//sky
	std::shared_ptr<Sky> skyObj;
	// Should we use vsync to limit the frame rate?
	bool vsync;
	float offset;
	// Initialization helper methods - feel free to customize, combine, etc.
	void LoadShaders(); 
	void CreateBasicGeometry();
	void LoadLights();
	void LoadTexturesSRVsAndSampler();
	void CreateEntitys();
	void OnResize();
	

	


};

