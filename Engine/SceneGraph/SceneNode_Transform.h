#pragma once

#include "D3D.h"
#include "SceneNode.h"

class SceneNode_Transform : public SceneNode
{
public:

	SceneNode_Transform(const DX::XMMATRIX& transform)
		: _transform(transform)
	{
	}

	virtual void PreExecute(SceneTraversalState& state, std::unique_ptr<SceneData>& sceneData) override;

protected:

	DX::XMMATRIX _transform;
};