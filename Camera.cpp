#include "Camera.h"

using namespace DirectX;

Camera::Camera(float x, float y, float z, float aspectRatio)
{
	transform.SetPosition(x, y, z);
	UpdateViewMatrix();
	UpdateProjectionMatrix(aspectRatio);
}

Camera::~Camera()
{
}

void Camera::Update(float dt)
{
}

void Camera::UpdateViewMatrix()
{
	//need the pos direction and world up vectors
	XMFLOAT3 pos = transform.GetPosition();
	XMFLOAT3 up = XMFLOAT3(0, 1, 0);
	XMFLOAT3 rot = transform.GetRotation();
	XMVECTOR forward = XMVector3Rotate(
		XMVectorSet(0, 0, 1, 0),
		XMQuaternionRotationRollPitchYaw(rot.x, rot.y, rot.z));


	XMMATRIX view = XMMatrixLookAtLH(
		XMLoadFloat3(&pos),
		forward,
		XMLoadFloat3(&up));

	//store our newly made matrix
	XMStoreFloat4x4(&viewMatrix, view);
}

//dont have to call every fram
void Camera::UpdateProjectionMatrix(float aspectRatio)
{
	//creating our projection matrix
	XMMATRIX proj = XMMatrixPerspectiveFovLH(
		XM_PIDIV4,
		aspectRatio,
		0.01f,  //near plane
		100.0f //far plane
	);



	XMStoreFloat4x4(&projectionMatrix, proj);
}

Transform* Camera::GetTransform()
{
	return &transform;
}

DirectX::XMFLOAT4X4 Camera::GetViewMatrix()
{
	return viewMatrix;
}

DirectX::XMFLOAT4X4 Camera::GetprojectionMatrix()
{
	return projectionMatrix;
}
