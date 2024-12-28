#pragma once

#include <DirectXMath.h>

struct Light
{
	DirectX::XMFLOAT4 diffuseColor;
	DirectX::XMFLOAT3 direction;
};