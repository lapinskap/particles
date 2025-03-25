#pragma once

#include "D3D.h"
#include "ReleasePtr.h"
#include "Shader.h"

class D3D;
struct Light;

class LightShader : public Shader
{
public:

	LightShader();

	virtual void Initialize(D3D& d3D) override;
	virtual std::unique_ptr<SceneData> CreateSceneData() const override;
	virtual void Render(D3D& d3D, const SceneData& sceneData) override;
	virtual size_t GetInstanceSize() const override;

protected:

	virtual std::vector<D3D11_INPUT_ELEMENT_DESC> GetVertexLayout() const override;

	ReleasePtr<ID3D11Buffer> _lightBuffer;
	ReleasePtr<ID3D11Buffer> _timeBuffer;
};