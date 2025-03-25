#include "LightShader.h"
#include "SceneGraph/SceneData.h"
#include "SceneGraph/SceneTraversalState.h"
#include "Light.h"
#include "InstancedModel.h"
#include "Model.h"

const WCHAR* vsFilename = L"../Engine/Light.vs";
const WCHAR* psFilename = L"../Engine/Light.ps";

struct MatrixBuffer
{
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

struct Instance
{
	DirectX::XMMATRIX transform;
};

LightShader::LightShader()
	: Shader(vsFilename, psFilename)
{
}

std::vector<D3D11_INPUT_ELEMENT_DESC> LightShader::GetVertexLayout() const
{
	// Create the vertex input layout description.
	// This setup needs to match the Vertex stucture in the Model and in the shader.
	std::vector<D3D11_INPUT_ELEMENT_DESC> vertexLayout
	{
		{ "POSITION",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	0,								D3D11_INPUT_PER_VERTEX_DATA,	0 },
		{ "COLOR",			0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
		{ "NORMAL",			0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
		{ "InstTransform",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	1,	0,								D3D11_INPUT_PER_INSTANCE_DATA,	1 },
		{ "InstTransform",	1, DXGI_FORMAT_R32G32B32A32_FLOAT,	1,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_INSTANCE_DATA,	1 },
		{ "InstTransform",	2, DXGI_FORMAT_R32G32B32A32_FLOAT,	1,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_INSTANCE_DATA,	1 },
		{ "InstTransform",	3, DXGI_FORMAT_R32G32B32A32_FLOAT,	1,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_INSTANCE_DATA,	1 }
	};
	return vertexLayout;
}

void LightShader::Initialize(D3D& d3D)
{
	Shader::Initialize(d3D);
	
	HRESULT result;

	auto device = d3D.GetDevice();

	D3D11_BUFFER_DESC matrixBufferDesc;
	ZeroMemory(&matrixBufferDesc, sizeof(matrixBufferDesc));
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBuffer);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	result = device->CreateBuffer(&matrixBufferDesc, nullptr, &_matrixBuffer);
	if (FAILED(result))
		throw D3DError("Failed to create a matrix buffer");

	D3D11_BUFFER_DESC viewBufferDesc;
	ZeroMemory(&viewBufferDesc, sizeof(viewBufferDesc));
	viewBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	viewBufferDesc.ByteWidth = sizeof(ViewBuffer);
	viewBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	viewBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
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
	result = device->CreateBuffer(&lightBufferDesc, nullptr, &_lightBuffer);
	if (FAILED(result))
		throw D3DError("Failed to create a light buffer");
}

struct LightShader_SceneData : public SceneData
{
	void AddInstanced(const SceneTraversalState& state) override
	{
		instances.push_back(Instance{ DX::XMMatrixTranspose(state.transform) });
		instanceCount++;
	}

	std::vector<Instance> instances;
	Light* light = nullptr;
};

std::unique_ptr<SceneData> LightShader::CreateSceneData() const
{
	return std::make_unique<LightShader_SceneData>();
}

void LightShader::Render(D3D& d3D, const SceneData& sceneData)
{
	const LightShader_SceneData& mySceneData = dynamic_cast<const LightShader_SceneData&>(sceneData);
	
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	auto viewMatrix = XMMatrixTranspose(mySceneData.viewMatrix);
	auto projectionMatrix = XMMatrixTranspose(mySceneData.projectionMatrix);

	auto deviceContext = d3D.GetDeviceContext();
	result = deviceContext->Map(_matrixBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
		throw D3DError("Can't map a resource");

	{
		MatrixBuffer* dataPtr = reinterpret_cast<MatrixBuffer*>(mappedResource.pData);
		dataPtr->view = viewMatrix;
		dataPtr->projection = projectionMatrix;
	}

	deviceContext->Unmap(_matrixBuffer.get(), 0);

	// Now set the constant buffer in the vertex shader with the updated values.
	// 0 - buffer index 1 - count
	deviceContext->VSSetConstantBuffers(0, 1, &_matrixBuffer);

	result = deviceContext->Map(_viewBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
		throw D3DError("Can't map a resource");

	{
		ViewBuffer* dataPtr = reinterpret_cast<ViewBuffer*>(mappedResource.pData);
		dataPtr->viewPosition = DX::XMFLOAT3(0, 0, 0);
	}

	deviceContext->Unmap(_viewBuffer.get(), 0);

	deviceContext->VSSetConstantBuffers(1, 1, &_viewBuffer);

	result = deviceContext->Map(_lightBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
		throw D3DError("Can't map a resource");

	{
		LightBuffer* dataPtr = reinterpret_cast<LightBuffer*>(mappedResource.pData);
		// TODO: dehardcode
		dataPtr->diffuseColor = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		dataPtr->lightDirection = DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f);
	}

	deviceContext->Unmap(_lightBuffer.get(), 0);
	deviceContext->PSSetConstantBuffers(0, 1, &_lightBuffer);

	mySceneData.instancedModel->ApplyBuffers(d3D, mySceneData.instances.data(), sizeof(Instance), mySceneData.instanceCount);

	deviceContext->IASetInputLayout(_layout.get());
	deviceContext->VSSetShader(_vertexShader.get(), nullptr, 0);
	deviceContext->PSSetShader(_pixelShader.get(), nullptr, 0);

	deviceContext->DrawIndexedInstanced(mySceneData.instancedModel->GetModel()->GetIndexCount(), mySceneData.instanceCount, 0, 0, 0);
}

size_t LightShader::GetInstanceSize() const
{
	return sizeof(Instance);
}