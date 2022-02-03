#pragma once
#include <DirectXMath.h>
class Transform
{
public:
	//constructor
	Transform();

	DirectX::XMFLOAT4X4 BuildMatrix();

	void MoveAbsolute(float x, float y, float z);
	void Rotate(float p, float y, float r);
	void Scale(float x, float y, float z);

	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetScale(float x, float y, float z);

	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetRotation();
	DirectX::XMFLOAT3 GetScale();

private:
	//raw transformation data
	//3 movements rotate scale pos
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 scale;
	DirectX::XMFLOAT3 pitchYallRoll;

	//finalized array
	DirectX::XMFLOAT4X4 worldMatrix;

	//does our matrix need a new update
	bool matrixDirty;
};

