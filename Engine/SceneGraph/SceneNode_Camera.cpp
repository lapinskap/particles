#include "SceneNode_Camera.h"
#include "SceneData.h"
#include "Camera.h"

SceneNode_Camera::SceneNode_Camera(Camera* camera)
	: _camera(camera)
{
}

void SceneNode_Camera::PreExecute(SceneTraversalState& state, std::unique_ptr<SceneData>& sceneData)
{
	_camera->Render();
	_camera->GetViewMatrix(sceneData->viewMatrix);
}