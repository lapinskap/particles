#pragma once

#include "SceneNode.h"

class Camera;
struct SceneData;
struct SceneTraversalState;

class SceneNode_Camera : public SceneNode
{
public:

	SceneNode_Camera(Camera* camera);

	virtual void PreExecute(SceneTraversalState& state, std::unique_ptr<SceneData>& sceneData) override;

protected:

	Camera* _camera;
};