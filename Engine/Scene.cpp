#include "Scene.h"
#include "D3D.h"
#include "Common.h"
#include "SceneGraph/SceneGraphExecutor.h"
#include "SceneGraph/SceneNode_InstancedModel.h"
#include "SceneGraph/SceneNode_Shader.h"
#include "SceneGraph/SceneNode_Transform.h"
#include "SceneGraph/SceneNode_Camera.h"
#include <iostream>

void Scene::Initialize(D3D& d3D)
{
	// Set the initial position of the camera.
	_camera.SetPosition(0.0f, 0.0f, -5.0f);

	auto instancedModel = std::make_shared<InstancedModel>(std::make_shared<SphereModel>());

	_rootSceneNode = std::make_unique<SceneNode>();

	auto sceneNode_Shader = _rootSceneNode->AddChild(new SceneNode_Shader(std::make_shared<LightShader>()));

	sceneNode_Shader
		->AddChild(new SceneNode_Transform(DX::XMMatrixScaling(0.5f, 0.5f, 0.5f)))
		->AddChild(new SceneNode_InstancedModel(instancedModel));

	_rotationalCenter = new SceneNode_Transform;
	sceneNode_Shader->AddChild(_rotationalCenter);

	_rotationalCenter
		->AddChild(new SceneNode_Transform(DX::XMMatrixTranslation(1.75f, 0.0f, 0.0f)))
		->AddChild(new SceneNode_Transform(DX::XMMatrixScaling(0.15f, 0.15f, 0.15f)))
		->AddChild(new SceneNode_InstancedModel(instancedModel));

	_rotationalCenter
		->AddChild(new SceneNode_Transform(DX::XMMatrixTranslation(-1.75f, 0.0f, 0.0f)))
		->AddChild(new SceneNode_Transform(DX::XMMatrixScaling(0.15f, 0.15f, 0.15f)))
		->AddChild(new SceneNode_InstancedModel(instancedModel));

	sceneNode_Shader->AddChild(new SceneNode_Camera(&_camera));

	_executor.Initialize(d3D, _rootSceneNode.get());
}

bool Scene::Render(D3D& d3D, float dt)
{
	static float rotation = 0.0f;
	rotation += dt;
	_rotationalCenter->transform = DX::XMMatrixRotationZ(rotation);

	_executor.Execute(_rootSceneNode.get());

	return true;
}



