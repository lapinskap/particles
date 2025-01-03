#include "Application.h"


Application::Application(uint screenWidth, uint screenHeight, HWND hwnd)
	: _direct3D(D3D::InitParams{ hwnd, screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH, VSYNC_ENABLED, FULL_SCREEN })
{
	// Set the initial position of the camera.
	_camera.SetPosition(0.0f, 0.0f, -5.0f);

	// Create and initialize the model object.
	_model = std::make_unique<SphereModel>();
	_model->Initialize(_direct3D);

	_lightShader = std::make_unique<LightShader>(_direct3D);

	_light.diffuseColor = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	_light.direction = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);
}

bool Application::Render(float rotation)
{
	DirectX::XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	// Clear the buffers to begin the scene.
	_direct3D.BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	_camera.Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	_direct3D.GetWorldMatrix(worldMatrix);
	_camera.GetViewMatrix(viewMatrix);
	_direct3D.GetProjectionMatrix(projectionMatrix);

	// Rotate the world matrix by the rotation value so that the triangle will spin.
	worldMatrix = DirectX::XMMatrixRotationY(rotation);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	_model->Render(_direct3D);

	// Render the model using the color shader.
	result = _lightShader->Render(_direct3D, _model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, _light);
	if (!result)
		return false;

	// Present the rendered scene to the screen.
	_direct3D.EndScene();

	return true;
}

bool Application::Frame()
{
	static float rotation = 0.0f;
	rotation -= 0.0174532925f * 0.1f;
	if (rotation < 0.0f)
	{
		rotation += 360.0f;
	}

	return Render(rotation);
}

