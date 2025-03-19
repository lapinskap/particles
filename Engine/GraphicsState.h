#pragma once

#include "Common.h"

class D3D;
class Model;
class Shader;

struct GraphicsState
{
	D3D* d3D = nullptr;
	Model* model = nullptr;
	Shader* shader = nullptr;
	DX::XMMATRIX worldMatrix;
	DX::XMMATRIX viewMatrix;
	DX::XMMATRIX projectionMatrix;
};