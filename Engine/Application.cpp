#include "Application.h"


Application::Application(uint screenWidth, uint screenHeight, HWND hwnd)
	: _direct3D(D3D::InitParams{ hwnd, screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH, VSYNC_ENABLED, FULL_SCREEN })
{
	_scene.Initialize(_direct3D);
	_lastUpdateTimepoint = std::chrono::steady_clock::now();
}

void Application::Frame()
{
	auto beginFrameTimepoint = std::chrono::steady_clock::now();

	// Clear the buffers to begin the scene.
	_direct3D.BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	std::chrono::duration<float> deltaTimeSeconds = beginFrameTimepoint - _lastUpdateTimepoint;
	_lastUpdateTimepoint = beginFrameTimepoint;

	_scene.Render(_direct3D, deltaTimeSeconds.count());

	// Present the rendered scene to the screen.
	_direct3D.EndScene();
}
