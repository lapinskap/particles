#pragma once

#include "SceneNode.h"

class InstancedModel;

class SceneNode_InstancedModel : public SceneNode
{
public:

	SceneNode_InstancedModel(std::shared_ptr<InstancedModel> instancedModel);

	virtual void PreInitialize(D3D& d3D, std::unique_ptr<SceneData>& sceneData) override;
	virtual void PreExecute(SceneTraversalState& state, std::unique_ptr<SceneData>& sceneData) override;

protected:

	std::shared_ptr<InstancedModel> _instancedModel;
};