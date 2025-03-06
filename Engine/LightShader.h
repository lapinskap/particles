#pragma once

#include <d3dcompiler.h>
#include "D3D.h"
#include "ReleasePtr.h"
#include "Shader.h"

class D3D;
struct Light;

class LightShader : public Shader
{
public:
	LightShader(D3D& d3D);

	bool Render_Old(D3D& d3D, int indexCount,
		DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix,
		Light& light, float time);

private:

	bool InitializeShader(D3D& d3D);
	D3D11_INPUT_ELEMENT_DESC renderVertexLayout(const char* semanticName, uint semanticIndex, DXGI_FORMAT format, uint alignedByteOffset);

	bool SetShaderParameters(D3D& d3D, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix,
										Light& light, float time);
	void RenderShader(D3D& d3D, int indexCount);

	void SetShaderParameters(D3D& d3D, const GraphicsState& graphicsState) override;
	
	ReleasePtr<ID3D11InputLayout> _layout;
	ReleasePtr<ID3D11Buffer> _lightBuffer;
	ReleasePtr<ID3D11Buffer> _timeBuffer;
};