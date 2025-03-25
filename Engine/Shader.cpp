#include "Shader.h"
#include <d3dcompiler.h>

Shader::Shader(const std::wstring& vertexShaderPath, const std::wstring& pixelShaderPath)
	: _vertexShaderPath(vertexShaderPath)
	, _pixelShaderPath(pixelShaderPath)
{
}

void Shader::Initialize(D3D& d3D)
{
	HRESULT result;
	ReleasePtr<ID3D10Blob> errorMessage;
	ReleasePtr<ID3D10Blob> vertexShaderBuffer;
	ReleasePtr<ID3D10Blob> pixelShaderBuffer;

	// Compile the vertex shader code.
	result = D3DCompileFromFile(_vertexShaderPath.c_str(), nullptr, nullptr, "LightVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
			throw D3DError(std::format("Error compiling shader\n{}", (const char*)errorMessage->GetBufferPointer()));
		else
			throw D3DError("Missing Shader File");
	}

	// Compile the pixel shader code.
	result = D3DCompileFromFile(_pixelShaderPath.c_str(), nullptr, nullptr, "LightPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
			throw D3DError(std::format("Error compiling shader\n{}", (const char*)errorMessage->GetBufferPointer()));
		else
			throw D3DError("Missing Shader File");
	}

	auto device = d3D.GetDevice();

	// Create the vertex shader from the buffer.
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), nullptr, &_vertexShader);
	if (FAILED(result))
		throw D3DError("Failed to create a vertex shader");

	// Create the pixel shader from the buffer.
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), nullptr, &_pixelShader);
	if (FAILED(result))
		throw D3DError("Failed to create a pixel shader");

	auto vertexLayout = GetVertexLayout();

	// Create the vertex input layout.
	result = device->CreateInputLayout(vertexLayout.data(), vertexLayout.size(), vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &_layout);
	if (FAILED(result))
		throw D3DError("Failed to create an input layout");
}

void Shader::Render(D3D& d3D, const SceneData& SceneData)
{
	
}

