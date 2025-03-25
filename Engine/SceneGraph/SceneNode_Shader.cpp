#include "SceneNode_Shader.h"
#include "SceneData.h"
#include "Shader.h"
#include "InstancedModel.h"

SceneNode_Shader::SceneNode_Shader(std::shared_ptr<Shader> shader)
	: _shader(shader)
{
}

void SceneNode_Shader::PreInitialize(D3D& d3D, std::unique_ptr<SceneData>& sceneData)
{
	_d3D = &d3D;
	_shader->Initialize(d3D);
	sceneData = _shader->CreateSceneData();
}

void SceneNode_Shader::PostInitialize(std::unique_ptr<SceneData>& sceneData)
{
	if (sceneData->instancedModel)
	{
		sceneData->instancedModel->Initialize(*_d3D, _shader->GetInstanceSize(), sceneData->instanceCount);
	}
}

void SceneNode_Shader::PreExecute(SceneTraversalState& state, std::unique_ptr<SceneData>& sceneData)
{
	sceneData = _shader->CreateSceneData();
	_d3D->GetProjectionMatrix(sceneData->projectionMatrix);
}

void SceneNode_Shader::PostExecute(const std::unique_ptr<SceneData>& sceneData)
{
	if (sceneData->instancedModel)
	{
		_shader->Render(*_d3D, *sceneData);
	}
}
