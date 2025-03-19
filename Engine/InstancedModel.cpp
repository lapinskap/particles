#include "InstancedModel.h"
#include "D3D.h"
#include "Common.h"
#include <math.h>
#include <iostream>

void InstancedModel::Initialize(D3D& d3D)
{
	InitializeBuffers(d3D);
}

void InstancedModel::Render(D3D& d3D)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(d3D);
}

int InstancedModel::GetIndexCount()
{
	return _indexCount;
}

std::vector<InstancedModel::Vertex> InstancedModel::CreateVertices() const
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

std::vector<uint> InstancedModel::CreateIndices() const
{
	return std::vector<uint> {0, 1, 2, 1, 3, 2};
}

int InstancedModel::GetVertexCount()
{
	return _vertexCount;
};

int InstancedModel::GetInstanceCount()
{
	return _instanceCount;
};

void InstancedModel::InitializeBuffers(D3D& d3D)
{
	HRESULT result;

	// Create the vertex array.
	std::vector<Vertex> vertices = CreateVertices();

	// Create the index array.
	std::vector<uint> indices = CreateIndices();

	// Set the number of instances in the array.
	_instanceCount = 4;

	// Create the instance array.
	Instance* instances = new Instance[_instanceCount];

	// Load the instance array with data.
	instances[0].position = DirectX::XMFLOAT3(-1.5f, -1.5f, 5.0f);
	instances[1].position = DirectX::XMFLOAT3(-1.5f, 1.5f, 5.0f);
	instances[2].position = DirectX::XMFLOAT3(1.5f, -1.5f, 5.0f);
	instances[3].position = DirectX::XMFLOAT3(1.5f, 1.5f, 5.0f);

	// Set up the description of the static vertex buffer.
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * vertices.size();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices.data();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	ID3D11Device* device = d3D.GetDevice();

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &_vertexBuffer);
	std::cout << "InitializeBuffers vertices count: " << vertices.size() << std::endl;

	if (FAILED(result))
		throw D3DError("Failed to create a vertex buffer");

	// Set up the description of the static index buffer.
	D3D11_BUFFER_DESC instanceBufferDesc;
	instanceBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	instanceBufferDesc.ByteWidth = sizeof(Instance) * _instanceCount;
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDesc.CPUAccessFlags = 0;
	instanceBufferDesc.MiscFlags = 0;
	instanceBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	D3D11_SUBRESOURCE_DATA instanceData;
	instanceData.pSysMem = instances;
	instanceData.SysMemPitch = 0;
	instanceData.SysMemSlicePitch = 0;

	// Create the instance buffer.
	result = device->CreateBuffer(&instanceBufferDesc, &instanceData, &_instanceBuffer);
	if (FAILED(result))
		throw D3DError("Failed to create an index buffer");

	_indexCount = indices.size();
	std::cout << "InitializeBuffers index count: " << _indexCount << std::endl;
}

void InstancedModel::RenderBuffers(D3D& d3D)
{
	// Set vertex buffer stride and offset.
	unsigned int strides[2];
	unsigned int offsets[2];
	ID3D11Buffer* bufferPointers[2];

	// Set the buffer strides.
	// The stride tells the GPU how many bytes to skip to get from one element to next.
	strides[0] = sizeof(Vertex);
	strides[1] = sizeof(Instance);

	// Set the buffer offsets.
	offsets[0] = 0;
	offsets[1] = 0;

	// Set the array of pointers to the vertex and instance buffers.
	bufferPointers[0] = _vertexBuffer.get();
	bufferPointers[1] = _instanceBuffer.get();

	auto deviceContext = d3D.GetDeviceContext();

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
