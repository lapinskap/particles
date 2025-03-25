#include "SceneGraphExecutor.h"
#include "SceneNode.h"
#include "SceneTraversalState.h"
#include "SceneData.h"
#include "InstancedModel.h"

void SceneGraphExecutor::Initialize(D3D& d3D, SceneNode* rootNode)
{
	std::unique_ptr<SceneData> sceneData;
	InitializeRecursively(d3D, rootNode, sceneData);
}

void SceneGraphExecutor::Execute(SceneNode* rootNode)
{
	SceneTraversalState sceneTraversalState;
	std::unique_ptr<SceneData> sceneData;
	ExecuteRecursively(rootNode, sceneTraversalState, sceneData);
}

void SceneGraphExecutor::InitializeRecursively(D3D& d3D, SceneNode* node, std::unique_ptr<SceneData>& sceneData)
{
	if (!node)
		return;

	node->PreInitialize(d3D, sceneData);
	for (auto child : node->GetChildren())
	{
		InitializeRecursively(d3D, child, sceneData);
	}
	node->PostInitialize(sceneData);
}

void SceneGraphExecutor::ExecuteRecursively(SceneNode* node, SceneTraversalState& sceneTraversalState, std::unique_ptr<SceneData>& sceneData)
{
	if (!node)
		return;

	node->PreExecute(sceneTraversalState, sceneData);
	auto& children = node->GetChildren();
	if (children.size() == 1)
	{
		ExecuteRecursively(children[0], sceneTraversalState, sceneData);
	}
	else
	{
		for (SceneNode* child : children)
		{
			auto sceneTraversalCopy = sceneTraversalState;
			ExecuteRecursively(child, sceneTraversalCopy, sceneData);
		}
	}
	node->PostExecute(sceneData);
}