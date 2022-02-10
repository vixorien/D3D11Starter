#pragma once
#include "Transform.h"
#include <DirectXMath.h>
class Camera
{
public:
	Camera(float x,float y,float z, float aspectRatio);
	~Camera();

	//update method
	void Update(float dt);
	void UpdateViewMatrix();
	void UpdateProjectionMatrix(float aspectRatio);

	//getter for the transform
	Transform* GetTransform();

	DirectX::XMFLOAT4X4 GetViewMatrix();
	DirectX::XMFLOAT4X4 GetprojectionMatrix();

private:
	//camera matrixes
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;

	Transform transform;

};

