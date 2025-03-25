#pragma once

#include "D3D.h"

struct SceneTraversalState
{
	SceneTraversalState()
	{
		transform = DX::XMMatrixIdentity();
	}

	DX::XMMATRIX transform;
};