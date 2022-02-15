#include "Transform.h"

using namespace DirectX;

Transform::Transform()
{
	SetPosition(0, 0, 0);
	SetScale(1, 1, 1);
	SetRotation(0, 0, 0);

	//create our initial matrix

	XMMATRIX ident = XMMatrixIdentity();
	XMStoreFloat4x4(&worldMatrix, ident);

}

DirectX::XMFLOAT4X4 Transform::BuildMatrix()
{
	//make sure we only do this work if we have too
	if (matrixDirty) 
	{
		//create the individual transform matrixes for each type of transform and then combine them and store the result
		XMMATRIX transMat = XMMatrixTranslation(position.x, position.y, position.z);
		XMMATRIX rotMat = XMMatrixRotationRollPitchYaw(pitchYawRoll.x, pitchYawRoll.y, pitchYawRoll.z);
		XMMATRIX scaleMat = XMMatrixScaling(scale.x, scale.y, scale.z);

		XMMATRIX worldMat = scaleMat * rotMat * transMat;
		//set our worldmatrix
		XMStoreFloat4x4(&worldMatrix, worldMat);

		//remember to clean matrix
		matrixDirty = false;
	}
	
	//
	return worldMatrix;
}

void Transform::MoveAbsolute(float x, float y, float z)
{
	//one way to do this is to just add the floats

	position.x += x;
	position.y += y;
	position.z += z;

	matrixDirty = true;
	//or use directxmath
	/*
	XMVECTOR pos = XMLoadFloat3(&position);
	XMVECTOR offset = XMVectorSet(x, y, z, 0);
	XMStoreFloat3(&position, pos + offset);
	*/
}

void Transform::MoveRelative(float x, float y, float z)
{
	//load the movment vector(initial)
	XMVECTOR moveVec = XMVectorSet(x, y, z, 0);

	//needs a quarternion and a vector
	XMVECTOR rotQuat = XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&pitchYawRoll));

	XMVECTOR rotatedVec = XMVector3Rotate(moveVec, rotQuat);

	//add the rotated movement vector to my position and overwrite my old position
	XMVECTOR newPos = XMLoadFloat3(&position) + rotatedVec;
	XMStoreFloat3(&position, newPos);

	//remember the matrixes are dirty
	matrixDirty = true;

}

void Transform::Rotate(float p, float y, float r)
{
	pitchYawRoll.x += p;
	pitchYawRoll.y += y;
	pitchYawRoll.z += r;

	matrixDirty = true;

}
/*
void Transform::Rotate(DirectX::XMFLOAT3 pitchYawRoll)
{
}
*/
void Transform::Scale(float x, float y, float z)
{
	scale.x *= x;
	scale.y *= y;
	scale.z *= z;

	matrixDirty = true;
}

void Transform::SetPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;

	matrixDirty = true;
}

void Transform::SetRotation(float x, float y, float z)
{
	pitchYawRoll.x = x;
	pitchYawRoll.y = y;
	pitchYawRoll.z = z;

	matrixDirty = true;

}

void Transform::SetScale(float x, float y, float z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;

	matrixDirty = true;
}

DirectX::XMFLOAT3 Transform::GetRight()
{
	XMFLOAT3 right;
	//get our forward vector
	XMVECTOR rightMath = XMVector3Rotate(
		XMVectorSet(1, 0, 0, 0),
		XMQuaternionRotationRollPitchYaw(pitchYawRoll.x, pitchYawRoll.y, pitchYawRoll.z));
	//store it in our float 3
	XMStoreFloat3(&right, rightMath);
	//return it
	return right;
}

DirectX::XMFLOAT3 Transform::GetForward()
{
	XMFLOAT3 forward;
	//get our forward vector
	XMVECTOR forwardMath = XMVector3Rotate(
		XMVectorSet(0, 0, 1, 0),
		XMQuaternionRotationRollPitchYaw(pitchYawRoll.x, pitchYawRoll.y, pitchYawRoll.z));
	//store it in our float 3
	XMStoreFloat3(&forward, forwardMath);
	//return it
	return forward;
}

DirectX::XMFLOAT3 Transform::GetUp()
{
	XMFLOAT3 up;
	//get our forward vector
	XMVECTOR upMath = XMVector3Rotate(
		XMVectorSet(0, 1, 0, 0),
		XMQuaternionRotationRollPitchYaw(pitchYawRoll.x, pitchYawRoll.y, pitchYawRoll.z));
	//store it in our float 3
	XMStoreFloat3(&up, upMath);
	//return it
	return up;
}

DirectX::XMFLOAT3 Transform::GetPosition()
{
	return position;
}

DirectX::XMFLOAT3 Transform::GetRotation()
{
	return pitchYawRoll;
}

DirectX::XMFLOAT3 Transform::GetScale()
{
	return scale;
}


