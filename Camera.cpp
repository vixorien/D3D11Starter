#include "Camera.h"
#include "Input.h"
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
	//cameras speed
	float speed = dt * 5.0f;

	//take in the user input
	Input& input = Input::GetInstance();

	//handle speed now
	if (input.KeyDown(VK_SHIFT)) { speed *= 5.0f; }
	if (input.KeyDown(VK_CONTROL)) { speed *= 0.1f; }

	//basic movements
	if (input.KeyDown('W')) { transform.MoveRelative(0, 0, speed); }
	if (input.KeyDown('A')) { transform.MoveRelative(-speed, 0, 0); }
	if (input.KeyDown('S')) { transform.MoveRelative(0, 0, -speed); }
	if (input.KeyDown('D')) { transform.MoveRelative(speed, 0, 0); }

	//up and down strafing
	if (input.KeyDown('X')) { transform.MoveAbsolute(0, -speed, 0); }
	if (input.KeyDown('Z')) { transform.MoveAbsolute(0, speed, 0); }

	//side to side strafing
	if (input.KeyDown('Q')) { transform.MoveAbsolute(-speed, 0, 0); }
	if (input.KeyDown('E')) { transform.MoveAbsolute(speed,0 , 0); }
	
	//handle mouse movement only when the left mouse button is down
	if (input.MouseLeftDown()) {
		
		float lookSpeed = dt * 5.0f;

		float xDiff = input.GetMouseXDelta() * lookSpeed;
		float yDiff = input.GetMouseYDelta() * lookSpeed;

		//now rotate the transform
		transform.Rotate(yDiff, xDiff, 0);
		
	}

	//make sure we send a call to update our view matrix
	UpdateViewMatrix();
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


	XMMATRIX view = XMMatrixLookToLH(
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
		XM_PIDIV2,
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

DirectX::XMFLOAT4X4 Camera::GetProjectionMatrix()
{
	return projectionMatrix;
}
