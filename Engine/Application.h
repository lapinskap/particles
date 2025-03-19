#pragma once

#include "Common.h"
#include "D3D.h"

#include "Scene.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.3f;

class System;

class Application
{
public:
	
	Application(uint screenWidth, uint screenHeight, HWND hwnd);

	bool Frame();

private:

	bool Render(float rotation);

	D3D _direct3D;
	Scene _scene;
	System* _system = nullptr;
};
