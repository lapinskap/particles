#pragma once

#include "Common.h"

class D3D;
struct SceneTraversalState;
struct SceneData;

class SceneNode
{
public:

	virtual ~SceneNode();

	SceneNode* AddChild(SceneNode* node);
	const std::vector<SceneNode*>& GetChildren() const { return _children; }

	// Called once for every node before the rendering begins, while traversing downwards (Depth First)
	// Shaders can create their SceneData so the nodes below fill it with content, can be used both for initialization and rendering purposes
	virtual void PreInitialize(D3D& d3D, std::unique_ptr<SceneData>& sceneData) {}
	// Called once for every node before the rendering begins, while traversing upwards (Depth First)
	virtual void PostInitialize(std::unique_ptr<SceneData>& sceneData) {}
	// Executing while traversing downwards (Depth First)
	virtual void PreExecute(SceneTraversalState& state, std::unique_ptr<SceneData>& sceneData) {}
	// Executing while going back upwards (so ShaderNode can finally draw)
	virtual void PostExecute(const std::unique_ptr<SceneData>&) {}

protected:

	std::vector<SceneNode*> _children;
};