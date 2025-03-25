#pragma once

#include "D3D.h"

struct SceneData;

class Shader
{
public:
	
	Shader(const std::wstring& vertexShaderPath, const std::wstring& pixelShaderPath);
	virtual ~Shader() = default;

	virtual void Initialize(D3D& d3D);
	virtual std::unique_ptr<SceneData> CreateSceneData() const = 0;
	virtual void Render(D3D& d3D, const SceneData& SceneData);
	virtual size_t GetInstanceSize() const { return 0u; }

protected:

	virtual std::vector<D3D11_INPUT_ELEMENT_DESC> GetVertexLayout() const = 0;

	std::wstring _vertexShaderPath;
	std::wstring _pixelShaderPath;
	ReleasePtr<ID3D11VertexShader> _vertexShader;
	ReleasePtr<ID3D11PixelShader> _pixelShader;
	ReleasePtr<ID3D11Buffer> _matrixBuffer;
	ReleasePtr<ID3D11Buffer> _viewBuffer;
	ReleasePtr<ID3D11InputLayout> _layout;
};