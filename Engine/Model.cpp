#include "Model.h"
#include "D3D.h"
#include "Common.h"
#include <math.h>
#include <iostream>

void Model::Initialize(D3D& d3D)
{
	InitializeBuffers(d3D);
}

void Model::Render(D3D& d3D)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(d3D);
}

int Model::GetVertexCount()
{
	return _vertexCount;
}

int Model::GetIndexCount()
{
	return _indexCount;
}

int Model::GetInstanceCount()
{
	return _instanceCount;
}

std::vector<Model::Vertex> Model::CreateVertices() const
{	
	// Load the vertex array with data.
	std::vector<Vertex> vertices(4);

	//// Load the vertex array with data.
	vertices[0].position = DirectX::XMFLOAT4(-1.0f, -1.0f, 0.0f, 1.0f);  // Bottom left
	vertices[0].color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	vertices[0].normal = DirectX::XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f);

	vertices[1].position = DirectX::XMFLOAT4(-1.0f, 1.0f, 0.0f, 1.0f);  // Top left
	vertices[1].color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	vertices[1].normal = DirectX::XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f);

	vertices[2].position = DirectX::XMFLOAT4(1.0f, -1.0f, 0.0f, 1.0f);  // Bottom right
	vertices[2].color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	vertices[2].normal = DirectX::XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f);

	vertices[3].position = DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f); // Top right
	vertices[3].color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	vertices[3].normal = DirectX::XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f);

	return vertices;
}

std::vector<uint> Model::CreateIndices() const
{
	return std::vector<uint> {0, 1, 2, 1, 3, 2};
}

void Model::InitializeBuffers(D3D& d3D)
{
	HRESULT result;

	// Create the vertex array.
	std::vector<Vertex> vertices = CreateVertices();

	// Create the index array.
	std::vector<uint> indices = CreateIndices();

	// Set up the description of the static vertex buffer.
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * (uint)vertices.size();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	// Give the subresource structure a pointer to the vertex data.
	D3D11_SUBRESOURCE_DATA vertexData;
	ZeroMemory(&vertexData, sizeof(vertexData));
	vertexData.pSysMem = vertices.data();

	ID3D11Device* device = d3D.GetDevice();

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &_vertexBuffer);
	std::cout << "InitializeBuffers vertices count: " << vertices.size() << std::endl;

	if (FAILED(result))
		throw D3DError("Failed to create a vertex buffer");

	std::vector<Instance> instances
	{
		{DX::XMFLOAT3(-1.5f, -1.5f, 0.0f)},
		{DX::XMFLOAT3(-1.5f, 1.5f, 0.0f)},
		{DX::XMFLOAT3(1.5f, -1.5f, 0.0f)}
	};

	_instanceCount = instances.size();

	D3D11_BUFFER_DESC instanceBufferDesc;
	ZeroMemory(&instanceBufferDesc, sizeof(instanceBufferDesc));
	instanceBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	instanceBufferDesc.ByteWidth = sizeof(Instance) * _instanceCount;
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA instanceData;
	ZeroMemory(&instanceData, sizeof(instanceData));
	instanceData.pSysMem = instances.data();

	result = device->CreateBuffer(&instanceBufferDesc, &instanceData, &_instanceBuffer);

	if (FAILED(result))
		throw D3DError("Failed to create an instance buffer");

	// Set up the description of the static index buffer.
	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(uint) * indices.size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	// Give the subresource structure a pointer to the index data.
	D3D11_SUBRESOURCE_DATA indexData;
	ZeroMemory(&indexData, sizeof(indexData));
	indexData.pSysMem = indices.data();

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &_indexBuffer);
	if (FAILED(result))
		throw D3DError("Failed to create an index buffer");

	_indexCount = indices.size();
	std::cout << "InitializeBuffers index count: " << _indexCount << std::endl;
}

void Model::RenderBuffers(D3D& d3D)
{
	// Set vertex buffer stride and offset.
	const uint strides[]{ sizeof(Vertex), sizeof(Instance) };
	const uint offsets[]{ 0u, 0u };
	ID3D11Buffer* bufferPointers[2] = {_vertexBuffer.get(), _instanceBuffer.get()};

	auto deviceContext = d3D.GetDeviceContext();

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);
	deviceContext->IASetIndexBuffer(_indexBuffer.get(), DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
