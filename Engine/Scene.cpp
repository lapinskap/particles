#include "Scene.h"
#include "D3D.h"
#include "Common.h"
#include "SceneGraph/SceneGraphExecutor.h"
#include "SceneGraph/SceneNode_InstancedModel.h"
#include "SceneGraph/SceneNode_Shader.h"
#include "SceneGraph/SceneNode_Transform.h"
#include "SceneGraph/SceneNode_Camera.h"

void Scene::Initialize(D3D& d3D)
{
	// Set the initial position of the camera.
	_camera.SetPosition(0.0f, 0.0f, -5.0f);

	auto instancedModel = std::make_shared<InstancedModel>(std::make_shared<SphereModel>());

	_rootSceneNode = std::make_unique<SceneNode>();

	auto sceneNode_Shader = _rootSceneNode->AddChild(new SceneNode_Shader(std::make_shared<LightShader>()));

	auto sceneNode_Camera = sceneNode_Shader->AddChild(new SceneNode_Camera(&_camera));

	sceneNode_Camera
		->AddChild(new SceneNode_Transform(DX::XMMatrixTranslation(0.0f, 1.0f, 0.0f)))
		->AddChild(new SceneNode_InstancedModel(instancedModel));

	sceneNode_Camera
		->AddChild(new SceneNode_Transform(DX::XMMatrixTranslation(-1.5f, -1.0f, 0.0f)))
		->AddChild(new SceneNode_InstancedModel(instancedModel));

	sceneNode_Camera
		->AddChild(new SceneNode_Transform(DX::XMMatrixTranslation(1.5f, -1.0f, 0.0f)))
		->AddChild(new SceneNode_InstancedModel(instancedModel));

	_executor.Initialize(d3D, _rootSceneNode.get());
}

bool Scene::Render(D3D& d3D, float dt)
{
	_executor.Execute(_rootSceneNode.get());

	return true;
}



