#pragma once

#include "Common.h"
#include "D3D.h"

#include "Camera.h"
#include "SphereModel.h"
#include "InstancedModel.h"
#include "LightShader.h"
#include "Light.h"
#include "SceneGraph/SceneNode.h"
#include "SceneGraph/SceneGraphExecutor.h"

class D3D;
class SceneGraphExecutor;
class SceneNode_Transform;

class Scene
{
public:

	void Initialize(D3D& d3D);
	bool Render(D3D& d3D, float dt);

	virtual ~Scene() = default;

private:

	SphereModel _model;
	Light _light;
	Camera _camera;
	std::unique_ptr<SceneNode> _rootSceneNode;
	SceneGraphExecutor _executor;
	SceneNode_Transform* _rotationalCenter;
};