#pragma once

#include "D3D.h"

class InstancedModel;
struct SceneTraversalState;

struct SceneData
{
	virtual ~SceneData() = default;

	virtual void AddInstanced(const SceneTraversalState& state) {}

	DX::XMMATRIX viewMatrix;
	DX::XMMATRIX projectionMatrix;
	InstancedModel* instancedModel = nullptr;
	uint instanceCount = 0u;
};