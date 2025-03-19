#pragma once

#include "Common.h"
#include "ReleasePtr.h"
#include "D3D.h"

struct GraphicsState;

class Shader
{
public:
	
	Shader(D3D& d3D, const s
td::wstring& vertexShaderPath, const std::wstring& pixelShaderPath);
	virtual ~Shader() = default;

	void Render(D3D& d3D, const GraphicsState& GraphicsState);

protected:
	virtual void SetShaderParameters(D3D& d3D, const GraphicsState& GraphicsState) = 0;
	//virtual void SetShaderParameters(ID3D11DeviceContext*, DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX, ID3D11ShaderResourceView*) = 0;

	ReleasePtr<ID3D11VertexShader> _vertexShader;
	ReleasePtr<ID3D11PixelShader> _pixelShader;
	ReleasePtr<ID3D11Buffer> _matrixBuffer;
	ReleasePtr<ID3D11Buffer> _viewBuffer;
};