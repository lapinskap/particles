#include "SceneNode_InstancedModel.h"
#include "SceneData.h"
#include "SceneTraversalState.h"

SceneNode_InstancedModel::SceneNode_InstancedModel(std::shared_ptr<InstancedModel> instancedModel)
	: _instancedModel(instancedModel)
{
}

void SceneNode_InstancedModel::PreInitialize(D3D& d3D, std::unique_ptr<SceneData>& sceneData)
{
	sceneData->instanceCount++;
	sceneData->instancedModel = _instancedModel.get();
}

void SceneNode_InstancedModel::PreExecute(SceneTraversalState& traversalState, std::unique_ptr<SceneData>& sceneData)
{
	sceneData->AddInstanced(traversalState);
	sceneData->instancedModel = _instancedModel.get();
}