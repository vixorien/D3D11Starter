#include "Game.h"
#include "Vertex.h"
#include "Input.h"
#include "PathHelpers.h"
#include "BufferStruct.h"


#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

// Needed for a helper function to load pre-compiled shader files
#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h>

// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// Direct3D itself, and our window, are not ready at this point!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
Game::Game(HINSTANCE hInstance)
	: DXCore(
		hInstance,			// The application's handle
		L"DirectX Game",	// Text for the window's title bar (as a wide-character string)
		1280,				// Width of the window's client area
		720,				// Height of the window's client area
		false,				// Sync the framerate to the monitor refresh? (lock framerate)
		true)				// Show extra stats (fps) in title bar?
{
	// Assignment 2
	windowColor[0] = 0.1f;
	windowColor[1] = 0.2f;
	windowColor[2] = 0.3f;
	windowColor[3] = 0.5f;
	showDemo = true;
	//meshes = std::vector<std::shared_ptr<Mesh>>();

#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.\n");
#endif
}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Delete all objects manually created within this class
//  - Release() all Direct3D objects created within this class
// --------------------------------------------------------
Game::~Game()
{
	// Call delete or delete[] on any objects or arrays you've
	// created using new or new[] within this class
	// - Note: this is unnecessary if using smart pointers

	// Call Release() on any Direct3D objects made within this class
	// - Note: this is unnecessary for D3D objects stored in ComPtrs

	// imgui
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

// --------------------------------------------------------
// Called once per program, after Direct3D and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these later
	LoadShaders();
	CreateGeometry();

	// Set initial graphics API state
	//  - These settings persist until we change them
	//  - Some of these, like the primitive topology & input layout, probably won't change
	//  - Others, like setting shaders, will need to be moved elsewhere later
	{
		// Tell the input assembler (IA) stage of the pipeline what kind of
		// geometric primitives (points, lines or triangles) we want to draw.  
		// Essentially: "What kind of shape should the GPU draw with our vertices?"
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Ensure the pipeline knows how to interpret all the numbers stored in
		// the vertex buffer. For this course, all of your vertices will probably
		// have the same layout, so we can just set this once at startup.
		context->IASetInputLayout(inputLayout.Get());

		// Set the active vertex and pixel shaders
		//  - Once you start applying different shaders to different objects,
		//    these calls will need to happen multiple times per frame
		context->VSSetShader(vertexShader.Get(), 0, 0);
		context->PSSetShader(pixelShader.Get(), 0, 0);
	}

	// Assignment 4
	//constantBuffer = device->CreateBuffer()
	{
		unsigned int size = sizeof(TintAndOffset);
		size = (size + 15) / 16 * 16;

		// describe buffer and create it
		D3D11_BUFFER_DESC cbDesc = {};
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.ByteWidth = size;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbDesc.MiscFlags = 0;
		cbDesc.StructureByteStride = 0;
		device->CreateBuffer(&cbDesc, 0, constantBuffer.GetAddressOf());
	
		// activate constant buffer, bind it to where the shader expects
		context->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
	}

	// Initialize ImGui itself & platform/renderer backends
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(device.Get(), context.Get());
	ImGui::StyleColorsDark();
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
	// BLOBs (or Binary Large OBjects) for reading raw data from external files
	// - This is a simplified way of handling big chunks of external data
	// - Literally just a big array of bytes read from a file
	ID3DBlob* pixelShaderBlob;
	ID3DBlob* vertexShaderBlob;

	// Loading shaders
	//  - Visual Studio will compile our shaders at build time
	//  - They are saved as .cso (Compiled Shader Object) files
	//  - We need to load them when the application starts
	{
		// Read our compiled shader code files into blobs
		// - Essentially just "open the file and plop its contents here"
		// - Uses the custom FixPath() helper from Helpers.h to ensure relative paths
		// - Note the "L" before the string - this tells the compiler the string uses wide characters
		D3DReadFileToBlob(FixPath(L"PixelShader.cso").c_str(), &pixelShaderBlob);
		D3DReadFileToBlob(FixPath(L"VertexShader.cso").c_str(), &vertexShaderBlob);

		// Create the actual Direct3D shaders on the GPU
		device->CreatePixelShader(
			pixelShaderBlob->GetBufferPointer(),	// Pointer to blob's contents
			pixelShaderBlob->GetBufferSize(),		// How big is that data?
			0,										// No classes in this shader
			pixelShader.GetAddressOf());			// Address of the ID3D11PixelShader pointer

		device->CreateVertexShader(
			vertexShaderBlob->GetBufferPointer(),	// Get a pointer to the blob's contents
			vertexShaderBlob->GetBufferSize(),		// How big is that data?
			0,										// No classes in this shader
			vertexShader.GetAddressOf());			// The address of the ID3D11VertexShader pointer
	}

	// Create an input layout 
	//  - This describes the layout of data sent to a vertex shader
	//  - In other words, it describes how to interpret data (numbers) in a vertex buffer
	//  - Doing this NOW because it requires a vertex shader's byte code to verify against!
	//  - Luckily, we already have that loaded (the vertex shader blob above)
	{
		D3D11_INPUT_ELEMENT_DESC inputElements[2] = {};

		// Set up the first element - a position, which is 3 float values
		inputElements[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;				// Most formats are described as color channels; really it just means "Three 32-bit floats"
		inputElements[0].SemanticName = "POSITION";							// This is "POSITION" - needs to match the semantics in our vertex shader input!
		inputElements[0].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;	// How far into the vertex is this?  Assume it's after the previous element

		// Set up the second element - a color, which is 4 more float values
		inputElements[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;			// 4x 32-bit floats
		inputElements[1].SemanticName = "COLOR";							// Match our vertex shader input!
		inputElements[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;	// After the previous element

		// Create the input layout, verifying our description against actual shader code
		device->CreateInputLayout(
			inputElements,							// An array of descriptions
			2,										// How many elements in that array?
			vertexShaderBlob->GetBufferPointer(),	// Pointer to the code of a shader that uses this layout
			vertexShaderBlob->GetBufferSize(),		// Size of the shader code that uses this layout
			inputLayout.GetAddressOf());			// Address of the resulting ID3D11InputLayout pointer
	}
}



// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Game::CreateGeometry()
{
	// Create some temporary variables to represent colors
	// - Not necessary, just makes things more readable
	XMFLOAT4 red	= XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green	= XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 blue	= XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	XMFLOAT4 brightGreen = XMFLOAT4(0.65f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 darkGreen = XMFLOAT4(0.0f, 0.30f, 0.02f, 1.0f);
	XMFLOAT4 cyan = XMFLOAT4(0.0f, 0.75f, 1.0f, 1.0f);

	Vertex vertices[] =
	{
		{ XMFLOAT3(+0.0f, +0.5f, +0.0f), red },
		{ XMFLOAT3(+0.5f, -0.5f, +0.0f), blue },
		{ XMFLOAT3(-0.5f, -0.5f, +0.0f), green },
	};
	Vertex shape1[] = {
		{XMFLOAT3(-0.25f, +0.25f, 0.0f), darkGreen},	// center
		{XMFLOAT3(-0.35f, +0.3f, 0.0f), brightGreen},	// top left
		{XMFLOAT3(-0.25f, +0.35f, 0.0f), brightGreen },	// top middle
		{XMFLOAT3(-0.15f, +0.3f, 0.0f), brightGreen},	// top right
		{XMFLOAT3(-0.15f, +0.15f, 0.0f), brightGreen},	// bottom right
		{XMFLOAT3(-0.35f, +0.15f, 0.0f), brightGreen},	// bottom left
	};
	Vertex shape2[] = {
		{XMFLOAT3(+0.7f, 0.0f, 0.0f), cyan},			// top left
		{XMFLOAT3(+0.9f, -0.1f, 0.0f), brightGreen},	// top right
		{XMFLOAT3(+0.15f, -0.9f, 0.0f), cyan},			// bottom left
		{XMFLOAT3(+0.25f, -1.0f, 0.0f), brightGreen},	// bottom right
	};

	unsigned int indices[] = { 0, 1, 2 };
	unsigned int indices1[] = { 
		0,1,2,	// center, up left, up middle
		0,2,3,	// center, up middle, up right
		0,3,4,	// center, up right, down right
		0,4,5,	// center, down right, down left
		0,5,1	// center, down left, up left
	};
	unsigned int indices2[] = { 
		0,1,3,	// top left, top right, bottom right
		0,3,2	// top left, bottom right, bottom left
	};


	// assignment 3
	std::shared_ptr<Mesh> s = std::make_shared<Mesh>(vertices, ARRAYSIZE(vertices), indices, ARRAYSIZE(indices), device);
	std::shared_ptr<Mesh> s1 = std::make_shared<Mesh>(shape1, ARRAYSIZE(shape1), indices1, ARRAYSIZE(indices1), device);
	std::shared_ptr<Mesh> s2 = std::make_shared<Mesh>(shape2, ARRAYSIZE(shape2), indices2, ARRAYSIZE(indices2), device);

	meshes.push_back(s);
	meshes.push_back(s1);
	meshes.push_back(s2);
}

void Game::RefreshImGui(float deltaTime) {
	// Put this all in a helper method that is called from Game::Update()
	// Feed fresh data to ImGui
	ImGuiIO& io = ImGui::GetIO();
	io.DeltaTime = deltaTime;
	io.DisplaySize.x = (float)this->windowWidth;
	io.DisplaySize.y = (float)this->windowHeight;
	// Reset the frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	// Determine new input capture
	Input& input = Input::GetInstance();
	input.SetKeyboardCapture(io.WantCaptureKeyboard);
	input.SetMouseCapture(io.WantCaptureMouse);
	// Show the demo window
	//ImGui::ShowDemoWindow();
}

void Game::BuildUI() {
	ImGui::Begin("Assignment 2 Dear ImGui");

	//static ImGuiTableFlags flags = ImGuiTableFlags_Hideable;
	//ImGui::CheckboxFlags("Hideable", &flags, ImGuiTableFlags_Hideable);

	// Replace the %f with the next parameter, and format as a float
	ImGui::Text("Framerate: %f fps", ImGui::GetIO().Framerate);
	// Replace each %d with the next parameter, and format as decimal integers
	// The "x" will be printed as-is between the numbers, like so: 800x600
	ImGui::Text("Window Resolution: %dx%d", windowWidth, windowHeight);

	// color picker
	ImGui::ColorPicker4("Color Picker", windowColor);
	// set window color
	//ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, windowColor, ) // needs u32???

	if (ImGui::Button("Hide/Show Demo", ImVec2(150, 50))) 
	{
		showDemo = !showDemo; 
	}
	//ImGui::ShowDemoWindow(&showDemo);
		
	if (showDemo)
	{
		ImGui::ShowDemoWindow();
	}

	ImGui::TextColored(ImVec4(1, 0.4, 0.3, 0.5), "Mouse x: %f", ImGui::GetIO().MousePos.x);
	
	static int wow;
	ImGui::SliderInt("I slide", &wow, 0, 100);

	//
	// Assignment 3
	//
	//ImGui::TreeNode("Assignment 3");
	//ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	//ImGui::TreeNode("Mesh 1: Triangle");
	int m1 = meshes[0]->GetVertexCount();
	int tris1 = m1 / 3;
	ImGui::Text("Mesh 1 Vertices: %i    Triangles: %i", m1, tris1);
	//ImGui::TreePop();

	int m2 = meshes[1]->GetVertexCount();
	int tris2 = m2 / 3;
	//ImGui::TreeNode("Mesh 2: Pentagram");
	ImGui::Text("Mesh 2 Vertices: %i    Triangles: %i", m2, tris2);
	//ImGui::TreePop();

	int m3 = meshes[2]->GetVertexCount();
	int tris3 = m3 / 3;
	//ImGui::TreeNode("Mesh 3: Rect");
	ImGui::Text("Mesh 3 Vertices: %i    Triangles: %i", m3, tris3);
	//ImGui::TreePop();

	//ImGui::TreePop();

	// Assignment 4
	ImGui::ColorPicker4("Color Tint Picker", colorTint);
	ImGui::DragFloat3("Offset", offset);

	ImGui::End();
}

// --------------------------------------------------------
// Handle resizing to match the new window size.
//  - DXCore needs to resize the back buffer
//  - Eventually, we'll want to update our 3D camera
// --------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	// update imgui
	RefreshImGui(deltaTime);
	BuildUI();

	// Example input checking: Quit if the escape key is pressed
	if (Input::GetInstance().KeyDown(VK_ESCAPE))
		Quit();
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Frame START
	// - These things should happen ONCE PER FRAME
	// - At the beginning of Game::Draw() before drawing *anything*
	{
		// Clear the back buffer (erases what's on the screen)
		//const float bgColor[4] = { 0.4f, 0.6f, 0.75f, 1.0f }; // Cornflower Blue
		context->ClearRenderTargetView(backBufferRTV.Get(), windowColor);

		// Clear the depth buffer (resets per-pixel occlusion information)
		context->ClearDepthStencilView(depthBufferDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	// Assignment 4
	// check back to notes
	TintAndOffset vsData;
	
	vsData.colorTint = DirectX::XMFLOAT4(colorTint);
	vsData.offset = DirectX::XMFLOAT3(offset);

	D3D11_MAPPED_SUBRESOURCE mappedBuffer = {};
	context->Map(constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer);

	// set color & offset
	memcpy(mappedBuffer.pData, &vsData, sizeof(vsData));
	
	context->Unmap(constantBuffer.Get(), 0);

	// copied from demo
	for (auto& m : meshes)
	{
		m->Draw(context);
	}

	// Frame END
	// - These should happen exactly ONCE PER FRAME
	// - At the very end of the frame (after drawing *everything*)
	{
		// Present the back buffer to the user
		//  - Puts the results of what we've drawn onto the window
		//  - Without this, the user never sees anything
		bool vsyncNecessary = vsync || !deviceSupportsTearing || isFullscreen;

		ImGui::Render(); // Turns this frame’s UI into renderable triangles
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData()); // Draws it to the screen

		swapChain->Present(
			vsyncNecessary ? 1 : 0,
			vsyncNecessary ? 0 : DXGI_PRESENT_ALLOW_TEARING);

		// Must re-bind buffers after presenting, as they become unbound
		context->OMSetRenderTargets(1, backBufferRTV.GetAddressOf(), depthBufferDSV.Get());
	}
}

