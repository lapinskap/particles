#include "Application.h"
#include "System.h"

Application::Application(uint screenWidth, uint screenHeight, HWND hwnd)
	: _direct3D(D3D::InitParams{ hwnd, screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH, VSYNC_ENABLED, FULL_SCREEN })
{
	_scene.Initialize(_direct3D);
	// Pass the camera pointer to the system
	_system->Initialize(&_scene.GetCamera());
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

	// Update the system to handle input and update the camera position
	_system->Run();

	return Render(rotation);
}

