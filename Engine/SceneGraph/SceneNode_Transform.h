#pragma once

#include "D3D.h"
#include "SceneNode.h"

class SceneNode_Transform : public SceneNode
{
public:

	SceneNode_Transform()
		: transform(DX::XMMatrixIdentity())
	{
	}

	SceneNode_Transform(const DX::XMMATRIX& transform_)
		: transform(transform_)
	{
	}

	virtual void PreExecute(SceneTraversalState& state, std::unique_ptr<SceneData>& sceneData) override;

	DX::XMMATRIX transform;
};