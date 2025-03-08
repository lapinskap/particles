#include "Scene.h"
#include "D3D.h"
#include "Common.h"
#include <math.h>
#include <iostream>

void Scene::Initialize(D3D& d3D)
{
	// Set the initial position of the camera.
	_camera.SetPosition(0.0f, 0.0f, -5.0f);

	// Create and initialize the model object.
	_model = std::make_unique<SphereModel>();
	_model->Initialize(d3D);

	_lightShader = std::make_unique<LightShader>(d3D);

	_light.diffuseColor = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	_light.direction = DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f);
}

bool Scene::Render(D3D& d3D, float rotation)
{

	DirectX::XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	// Generate the view matrix based on the camera's position.
	_camera.Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	d3D.GetWorldMatrix(worldMatrix);
	_camera.GetViewMatrix(viewMatrix);
	d3D.GetProjectionMatrix(projectionMatrix);

	// Rotate the world matrix by the rotation value so that the triangle will spin.
	worldMatrix = DirectX::XMMatrixRotationY(rotation);

	// Transpose the matrices to prepare them for the shader.	
	std::vector<DirectX::XMMATRIX> worldMatrices = {
		XMMatrixTranspose(worldMatrix * DirectX::XMMatrixTranslation(-2.0f, 0.0f, 0.0f)),
		XMMatrixTranspose(worldMatrix * DirectX::XMMatrixTranslation(-0.5f, -1.0f, 0.0f)),
		XMMatrixTranspose(worldMatrix * DirectX::XMMatrixTranslation(0.0f, 2.0f, 0.0f))
	};

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	_model->Render(d3D);

	// Render the model using the color shader.
	bool result = _lightShader->Render_Old(d3D, _model->GetIndexCount(), worldMatrices, viewMatrix, projectionMatrix, _light, rotation);
	if (!result)
		return false;

	return true;
}



