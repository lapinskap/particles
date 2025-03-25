#pragma once

#include "D3D.h"

class SceneNode;
struct SceneTraversalState;
struct SceneData;

class SceneGraphExecutor
{
public:

	void Initialize(D3D& d3D, SceneNode* rootNode);

	void Execute(SceneNode* rootNode);

private:

	void InitializeRecursively(D3D& d3D, SceneNode* node, std::unique_ptr<SceneData>& sceneData);
	void ExecuteRecursively(SceneNode* node, SceneTraversalState& sceneTraversalState, std::unique_ptr<SceneData>& sceneData);

	SceneNode* _rootNode = nullptr;
};