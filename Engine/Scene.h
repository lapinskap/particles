#pragma once
#include "Common.h"
#include "D3D.h"

#include "Camera.h"
#include "Model.h"
#include "SphereModel.h"
#include "LightShader.h"
#include "Light.h"

class D3D;

class Scene
{
public:

	void Initialize(D3D& d3D);
	bool Render(D3D& d3D, float rotation);

	virtual ~Scene() = default;

private:

	std::vector<std::unique_ptr<SphereModel>> _models;
	std::unique_ptr<LightShader> _lightShader;
	Light _light;
	Camera _camera;
};