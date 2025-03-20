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

	bool Render_Old(D3D& d3D, int indexCount, int instanceCount,
		DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix,
		Light& light);

private:

	bool InitializeShader(D3D& d3D);

	bool SetShaderParameters(D3D& d3D, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix, Light& light);
	void RenderShader(D3D& d3D, int indexCount, int instanceCount, DirectX::XMMATRIX worldMatrix);

	ReleasePtr<ID3D11InputLayout> _layout;
	ReleasePtr<ID3D11Buffer> _lightBuffer;
	ReleasePtr<ID3D11Buffer> _timeBuffer;
};