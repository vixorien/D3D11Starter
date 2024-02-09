#include "BufferStruct.h"

BufferStruct::BufferStruct(DirectX::XMFLOAT4 color, DirectX::XMFLOAT3 offset)
{
	this->colorTint = color;
	this->offset = offset;
}

BufferStruct::~BufferStruct()
{

}

void BufferStruct::SetColorTint(DirectX::XMFLOAT4 color)
{
	this->colorTint = color;
}

void BufferStruct::SetOffset(DirectX::XMFLOAT3 offset)
{
	this->offset = offset;
}

DirectX::XMFLOAT4 BufferStruct::GetColorTint()
{
	return colorTint;
}

DirectX::XMFLOAT3 BufferStruct::GetOffset()
{
	return offset;
}
