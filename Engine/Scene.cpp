#include "Scene.h"
#include "D3D.h"
#include "Common.h"

void Scene::Initialize(D3D& d3D)
{
	// Set the initial position of the camera.
	_camera.SetPosition(0.0f, 0.0f, -5.0f);

	_model.Initialize(d3D);

	std::vector<InstancedModel::Instance> instances
	{
		{{0.0f, 1.0f, 0.0f}},
		{{-1.5f, -1.0f, 0.0f}},
		{{1.5f, -1.0f, 0.0f}},
	};

	_instancedModel.Initialize(d3D, &_model, instances);

	_lightShader = std::make_unique<LightShader>(d3D);

	_light.diffuseColor = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	_light.direction = DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f);
}

bool Scene::Render(D3D& d3D, float dt)
{
	DirectX::XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	// Generate the view matrix based on the camera's position.
	_camera.Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	d3D.GetWorldMatrix(worldMatrix);
	_camera.GetViewMatrix(viewMatrix);
	d3D.GetProjectionMatrix(projectionMatrix);

	static float time = 0.0f;
	time += dt;

	float rotation = 0.1f * time;

	// Rotate the world matrix by the rotation value so that the triangle will spin.
	worldMatrix = DirectX::XMMatrixRotationY(rotation);

	_instancedModel.ApplyBuffers(d3D);

	// Render the model using the light shader.
	bool result = _lightShader->Render_Old(d3D, 
										   _model.GetIndexCount(), _instancedModel.GetInstanceCount(),
										   worldMatrix, viewMatrix, projectionMatrix, 
										   _light);
	if (!result)
		return false;

	return true;
}



