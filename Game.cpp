#include "Game.h"
#include <iostream>
#include "DXCore.h"
#include "Vertex.h"
#include "Input.h"
#include "d3dcompiler.h"
#include "BufferStructs.h"
#include "GameEntity.h"
#include "Material.h"

// Needed for a helper function to read compiled shader files from the hard drive
#pragma comment(lib, "d3dcompiler.lib")
// For the DirectX Math library
using namespace DirectX;
// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
Game::Game(HINSTANCE hInstance)
	: DXCore(
		hInstance,		   // The application's handle
		"DirectX Game",	   // Text for the window's title bar
		1280,			   // Width of the window's client area
		720,			   // Height of the window's client area
		true),			   // Show extra stats (fps) in title bar?
	//call transform constructor
	transform(),
	vsync(false)
{
#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.\n");
#endif

}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{
	// Note: Since we're using smart pointers (ComPtr),
	// we don't need to explicitly clean up those DirectX objects
	// - If we weren't using smart pointers, we'd need
	//   to call Release() on each DirectX object created in Game

	//make sure we offload our entities in our constructor
	for (auto& e : listOfEntitys) { delete e; }


}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these later
	LoadShaders();

	mat1 = std::make_shared<Material>(vertexShader, pixelShader,XMFLOAT3(1,1,1),.9f);
	//mat2 = std::make_shared<Material>(vertexShader, pixelShader2, XMFLOAT3(1,1,0));

	CreateBasicGeometry();

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//create our camera
	camera = std::make_shared<Camera>(0.0f, 0.0f, -5.0f, (float)width / height);

	ambientColor = XMFLOAT3(0.1f, 0.1f, 0.25f);
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files
// and also created the Input Layout that describes our 
// vertex data to the rendering pipeline. 
// - Input Layout creation is done here because it must 
//    be verified against vertex shader byte code
// - We'll have that byte code already loaded below
// --------------------------------------------------------
void Game::LoadShaders()
{
	vertexShader = std::make_shared<SimpleVertexShader>(device, context, GetFullPathTo_Wide(L"VertexShader.cso").c_str());
	pixelShader = std::make_shared<SimplePixelShader>(device, context, GetFullPathTo_Wide(L"PixelShader.cso").c_str());
	pixelShader2 = std::make_shared<SimplePixelShader>(device, context, GetFullPathTo_Wide(L"CustomPS.cso").c_str());
}



// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Game::CreateBasicGeometry()
{
	// Create some temporary variables to represent colors
	// - Not necessary, just makes things more readable
	shapeOne = std::make_shared<Mesh>(GetFullPathTo("../../Assets/Models/sphere.obj").c_str(), device,context);

	//int me = shapeOne->GetIndexCount();
	//std::cout << me;
	shapeTwo = std::make_shared<Mesh>(GetFullPathTo("../../Assets/Models/torus.obj").c_str(), device,context);
	shapeThree = std::make_shared<Mesh>(GetFullPathTo("../../Assets/Models/cube.obj").c_str(), device,context);
	
	//creating our 5 entitys
	GameEntity* entityOne = new GameEntity(shapeOne.get(),mat1);
	GameEntity* entityTwo = new GameEntity(shapeTwo.get(),mat1);
	GameEntity* entityThree = new  GameEntity(shapeThree.get(),mat1);
	GameEntity* entityFour = new GameEntity(shapeThree.get(),mat1);
	GameEntity* entityFive = new GameEntity(shapeThree.get(),mat1);
	
	//pushing entitys to list
	listOfEntitys.push_back(entityOne);
	listOfEntitys.push_back(entityTwo);
	listOfEntitys.push_back(entityThree);
	listOfEntitys.push_back(entityFour);
	listOfEntitys.push_back(entityFive);
	
	//making sure we put them in a good spot
	listOfEntitys[0]->getTransform()->SetPosition(0, 0, 0);
	listOfEntitys[1]->getTransform()->SetPosition(-2.5, 0, 0);
	listOfEntitys[2]->getTransform()->SetPosition(2.5, 0, 0);
	listOfEntitys[3]->getTransform()->SetPosition(-5.5, 0, 0);
	listOfEntitys[4]->getTransform()->SetPosition(5.5, 0, 0);
}


// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();
	//make sure we update our projection matrix when the screen resizes
	camera->UpdateProjectionMatrix((float)this->width / this->height);
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	// Example input checking: Quit if the escape key is pressed
	if (Input::GetInstance().KeyDown(VK_ESCAPE))
		Quit();

	//make sure we update our camera
	camera->Update(deltaTime);
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV.Get(), color);
	context->ClearDepthStencilView(
		depthStencilView.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);
	
	//loop through and draw our entitys
	for (int i = 0; i < listOfEntitys.size(); i++) {
		//going to pass this jawn over to our shader here because for some reason this doesnt belong in entity class but wouldnt it make more sense to pass the ambient color into the entity instead of creating a seperation of tasks that just doesnt make a whole lot of sense, Yeah i get it, this is probably a little less cpu power but im not sure if its worth the loss in coesive code
		listOfEntitys[i]->GetMaterial()->GetPixelShader()->SetFloat3("ambient", ambientColor);
		listOfEntitys[i]->Draw(context, camera);
	}
	
	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(vsync ? 1 : 0, 0);

	// Due to the usage of a more sophisticated swap chain,
	// the render target must be re-bound after every call to Present()
	context->OMSetRenderTargets(1, backBufferRTV.GetAddressOf(), depthStencilView.Get());
}