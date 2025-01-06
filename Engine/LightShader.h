#pragma once

#include <d3dcompiler.h>
#include "D3D.h"
#include "ReleasePtr.h"

class D3D;
struct Light;

class LightShader
{
public:
	LightShader(D3D& d3D);

	bool Render(D3D& d3D, int indexCount,
		DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix,
		Light& light, float time);

private:

	bool InitializeShader(D3D& d3D);

	bool SetShaderParameters(D3D& d3D, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix,
										Light& light, float time);
	void RenderShader(D3D& d3D, int indexCount);

	ReleasePtr<ID3D11VertexShader> _vertexShader;
	ReleasePtr<ID3D11PixelShader> _pixelShader;
	ReleasePtr<ID3D11InputLayout> _layout;
	ReleasePtr<ID3D11SamplerState> _sampleState;
	ReleasePtr<ID3D11Buffer> _matrixBuffer;
	ReleasePtr<ID3D11Buffer> _lightBuffer;
	ReleasePtr<ID3D11Buffer> _timeBuffer;
};