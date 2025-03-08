#include "LightShader.h"

#include "Light.h"

const WCHAR* vsFilename = L"../Engine/Light.vs";
const WCHAR* psFilename = L"../Engine/Light.ps";

struct MatrixBuffer
{
	DirectX::XMMATRIX world[10];
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;
};

struct ViewBuffer
{
	DirectX::XMFLOAT3 viewPosition;
	float padding;
};

struct LightBuffer
{
	DirectX::XMFLOAT4 diffuseColor;
	DirectX::XMFLOAT3 lightDirection;
	float padding;
};

struct TimeBuffer
{
	float time;
	float padding[3];
};

LightShader::LightShader(D3D& d3D)
	: Shader(d3D, vsFilename, psFilename)
{
	InitializeShader(d3D);
}

bool LightShader::Render_Old(D3D& d3D, int indexCount,
	const std::vector<DirectX::XMMATRIX>& worldMatrices, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix,
						Light& light, float time)
{
	// Set the shader parameters that it will use for rendering.
	bool result = SetShaderParameters(d3D, worldMatrices, viewMatrix, projectionMatrix, light, time);
	if (!result)
		return false;

	// Now render the prepared buffers with the shader.
	RenderShader(d3D, indexCount, worldMatrices);

	return true;
}

D3D11_INPUT_ELEMENT_DESC LightShader::renderVertexLayout(const char* semanticName, uint semanticIndex, DXGI_FORMAT format, uint alignedByteOffset)
{
	D3D11_INPUT_ELEMENT_DESC vertexLayout;
	vertexLayout.SemanticName = semanticName;
	vertexLayout.SemanticIndex = semanticIndex;
	vertexLayout.Format = format;
	vertexLayout.InputSlot = 0;
	vertexLayout.AlignedByteOffset = alignedByteOffset;
	vertexLayout.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	vertexLayout.InstanceDataStepRate = 0;

	return vertexLayout;
}


bool LightShader::InitializeShader(D3D& d3D)
{
	HRESULT result;
	ReleasePtr<ID3D10Blob> errorMessage;
	ReleasePtr<ID3D10Blob> vertexShaderBuffer;
	ReleasePtr<ID3D10Blob> pixelShaderBuffer;

	// Initialize the pointers this function will use to null.

	// Compile the vertex shader code.
	result = D3DCompileFromFile(vsFilename, nullptr, nullptr, "LightVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
			throw D3DError(std::format("Error compiling shader\n{}", (const char*)errorMessage->GetBufferPointer()));
		else
			throw D3DError("Missing Shader File");
	}

	// Compile the pixel shader code.
	result = D3DCompileFromFile(psFilename, nullptr, nullptr, "LightPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage);
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

	// Create the vertex input layout description.
	// This setup needs to match the Vertex stucture in the Model and in the shader.
	D3D11_INPUT_ELEMENT_DESC vertexLayout[4];

	vertexLayout[0] = renderVertexLayout("POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	vertexLayout[1] = renderVertexLayout("COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_APPEND_ALIGNED_ELEMENT);
	vertexLayout[2] = renderVertexLayout("NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_APPEND_ALIGNED_ELEMENT);
	vertexLayout[3] = renderVertexLayout("SV_InstanceID", 0, DXGI_FORMAT_R32_UINT, D3D11_APPEND_ALIGNED_ELEMENT);

	// Get a count of the elements in the layout.
	uint numElements = sizeof(vertexLayout) / sizeof(vertexLayout[0]);

	// Create the vertex input layout.
	result = device->CreateInputLayout(vertexLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &_layout);
	if (FAILED(result))
		throw D3DError("Failed to create an input layout");

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	D3D11_BUFFER_DESC matrixBufferDesc;
	ZeroMemory(&matrixBufferDesc, sizeof(matrixBufferDesc));
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBuffer);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&matrixBufferDesc, nullptr, &_matrixBuffer);
	if (FAILED(result))
		throw D3DError("Failed to create a matrix buffer");

	D3D11_BUFFER_DESC viewBufferDesc;
	ZeroMemory(&viewBufferDesc, sizeof(viewBufferDesc));
	viewBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	viewBufferDesc.ByteWidth = sizeof(ViewBuffer);
	viewBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	viewBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	viewBufferDesc.MiscFlags = 0;
	viewBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&viewBufferDesc, nullptr, &_viewBuffer);
	if (FAILED(result))
		throw D3DError("Failed to create a view buffer");

	// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
	D3D11_BUFFER_DESC lightBufferDesc;
	ZeroMemory(&lightBufferDesc, sizeof(lightBufferDesc));
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBuffer);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&lightBufferDesc, nullptr, &_lightBuffer);
	if (FAILED(result))
		throw D3DError("Failed to create a light buffer");

	D3D11_BUFFER_DESC timeBufferDesc;
	ZeroMemory(&timeBufferDesc, sizeof(timeBufferDesc));
	timeBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	timeBufferDesc.ByteWidth = sizeof(TimeBuffer);
	timeBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	timeBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	timeBufferDesc.MiscFlags = 0;
	timeBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&timeBufferDesc, nullptr, &_timeBuffer);
	if (FAILED(result))
		throw D3DError("Failed to create a time buffer");

	return true;
}

bool LightShader::SetShaderParameters(D3D& d3D, const std::vector<DirectX::XMMATRIX>& worldMatrices, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix,
												Light& light, float time)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;


	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	auto deviceContext = d3D.GetDeviceContext();

	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map(_matrixBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
		return false;

	{
		// Get a pointer to the data in the constant buffer.
		MatrixBuffer* dataPtr = reinterpret_cast<MatrixBuffer*>(mappedResource.pData);

		// Copy the matrices into the constant buffer.
		for (int i = 0; i < worldMatrices.size(); i++)
			dataPtr->world[i] = worldMatrices[i];

		dataPtr->view = viewMatrix;
		dataPtr->projection = projectionMatrix;
	}

	// Unlock the constant buffer.
	deviceContext->Unmap(_matrixBuffer.get(), 0);

	// Now set the constant buffer in the vertex shader with the updated values.
	// 0 - buffer index? 1 - count?
	deviceContext->VSSetConstantBuffers(0, 1, &_matrixBuffer);

	result = deviceContext->Map(_viewBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
		return false;

	{
		ViewBuffer* dataPtr = reinterpret_cast<ViewBuffer*>(mappedResource.pData);
		dataPtr->viewPosition = DX::XMFLOAT3(0, 0, 0);
	}

	// Unlock the constant buffer.
	deviceContext->Unmap(_viewBuffer.get(), 0);

	// Finally set the light constant buffer in the pixel shader with the updated values.
	deviceContext->VSSetConstantBuffers(1, 1, &_viewBuffer);

	// Lock the light constant buffer so it can be written to.
	result = deviceContext->Map(_lightBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
		return false;

	{
		// Get a pointer to the data in the constant buffer.
		LightBuffer* dataPtr = reinterpret_cast<LightBuffer*>(mappedResource.pData);

		// Copy the lighting variables into the constant buffer.
		dataPtr->diffuseColor = light.diffuseColor;
		dataPtr->lightDirection = light.direction;
	}

	// Unlock the constant buffer.
	deviceContext->Unmap(_lightBuffer.get(), 0);

	// Finally set the light constant buffer in the pixel shader with the updated values.
	deviceContext->PSSetConstantBuffers(0, 1, &_lightBuffer);

	result = deviceContext->Map(_timeBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
		return false;

	{
		// Get a pointer to the data in the constant buffer.
		TimeBuffer* dataPtr = reinterpret_cast<TimeBuffer*>(mappedResource.pData);

		// Copy the lighting variables into the constant buffer.
		dataPtr->time = time;
	}


	// Unlock the constant buffer.
	deviceContext->Unmap(_timeBuffer.get(), 0);

	// Finally set the light constant buffer in the pixel shader with the updated values.
	deviceContext->PSSetConstantBuffers(1, 1, &_timeBuffer);

	return true;
}


void LightShader::RenderShader(D3D& d3D, int indexCount, const std::vector<DirectX::XMMATRIX>& worldMatrices)
{
	auto deviceContext = d3D.GetDeviceContext();
	// Set the vertex input layout.
	deviceContext->IASetInputLayout(_layout.get());

	// Set the vertex and pixel shaders that will be used to render this triangle.
	deviceContext->VSSetShader(_vertexShader.get(), nullptr, 0);
	deviceContext->PSSetShader(_pixelShader.get(), nullptr, 0);

	//deviceContext->DrawIndexed(indexCount, 0, 0);
	// vector.size(), of matrixes
	deviceContext->DrawIndexedInstanced(indexCount, worldMatrices.size(), 0, 0, 0);
}

void LightShader::SetShaderParameters(D3D& d3D, const GraphicsState& graphicsState)
{

}