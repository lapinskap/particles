#include "Scene.h"
#include "D3D.h"
#include "Common.h"
#include <math.h>
#include <iostream>

void Scene::Initialize(D3D& d3D)
{
	// Set the initial position of the camera.
	_camera.SetPosition(0.0f, 0.0f, -5.0f);

	_model.Initialize(d3D);

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

	static float rotation = 0.0f;
	rotation += 0.1f * dt;
	if (rotation > 360.0f)
		rotation = 0.0f;

	// Rotate the world matrix by the rotation value so that the triangle will spin.
	worldMatrix = DirectX::XMMatrixRotationY(rotation);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	_model.Render(d3D);

	// Render the model using the light shader.
	bool result = _lightShader->Render_Old(d3D, 
										   _model.GetVertexCount(), _model.GetIndexCount(), _model.GetInstanceCount(), 
										   worldMatrix, viewMatrix, projectionMatrix, 
										   _light, rotation);
	if (!result)
		return false;

	return true;
}



