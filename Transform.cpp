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
		XMMATRIX rotMat = XMMatrixRotationRollPitchYaw(pitchYallRoll.x, pitchYallRoll.y, pitchYallRoll.z);
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

void Transform::Rotate(float p, float y, float r)
{
	pitchYallRoll.x += p;
	pitchYallRoll.y += y;
	pitchYallRoll.z += r;

	matrixDirty = true;

}

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
	pitchYallRoll.x = x;
	pitchYallRoll.y = y;
	pitchYallRoll.z = z;

	matrixDirty = true;

}

void Transform::SetScale(float x, float y, float z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;

	matrixDirty = true;
}

DirectX::XMFLOAT3 Transform::GetPosition()
{
	return position;
}

DirectX::XMFLOAT3 Transform::GetRotation()
{
	return pitchYallRoll;
}

DirectX::XMFLOAT3 Transform::GetScale()
{
	return scale;
}
