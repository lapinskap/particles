#include "Application.h"


Application::Application(uint screenWidth, uint screenHeight, HWND hwnd)
	: _direct3D(D3D::InitParams{ hwnd, screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH, VSYNC_ENABLED, FULL_SCREEN })
{
	_scene.Initialize(_direct3D);
}

bool Application::Render(float rotation)
{
	DirectX::XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	// Clear the buffers to begin the scene.
	_direct3D.BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	_scene.Render(_direct3D, rotation);

	// Present the rendered scene to the screen.
	_direct3D.EndScene();

	return true;
}

bool Application::Frame()
{
	static float rotation = 0.0f;
	rotation += 0.0174532925f * 0.1f;
	if (rotation > 360.0f)
	{
		rotation = 0.0f;
	}

	return Render(rotation);
}

