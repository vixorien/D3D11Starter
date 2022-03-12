#pragma once
#include <DirectXMath.h>
class Transform
{
public:
	//constructor
	Transform();

	DirectX::XMFLOAT4X4 BuildMatrix();

	void MoveAbsolute(float x, float y, float z);
	void MoveRelative(float x, float y, float z);
	void Rotate(float p, float y, float r);
	void Rotate(DirectX::XMFLOAT3 pitchYawRoll);
	void Scale(float x, float y, float z);

	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetScale(float x, float y, float z);

	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetRotation();
	DirectX::XMFLOAT3 GetScale();

	DirectX::XMFLOAT3 GetRight();
	DirectX::XMFLOAT3 GetForward();
	DirectX::XMFLOAT3 GetUp();

	DirectX::XMFLOAT4X4 GetWorldInverseTranspose();

private:
	//raw transformation data
	//3 movements rotate scale pos
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 scale;
	DirectX::XMFLOAT3 pitchYawRoll;

	//finalized array
	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMFLOAT4X4 worldInverseTransposeMatrix;
	//does our matrix need a new update
	bool matrixDirty;
};

