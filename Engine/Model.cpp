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

int Model::GetIndexCount()
{
	return _indexCount;
}

std::vector<Model::Vertex> Model::CreateVertices() const
{
	std::vector<Vertex> vertices(4);

	// Load the vertex array with data.
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

	std::cout << "Model CreateVertices" << std::endl;


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
	if (FAILED(result))
		throw D3DError("Failed to create a vertex buffer");

	// Set up the description of the static index buffer.
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(uint) * indices.size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices.data();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &_indexBuffer);
	if (FAILED(result))
		throw D3DError("Failed to create an index buffer");

	_indexCount = indices.size();
	std::cout << "Model index count: " << _indexCount << std::endl;
}

void Model::RenderBuffers(D3D& d3D)
{
	// Set vertex buffer stride and offset.
	const uint stride = sizeof(Vertex);
	const uint offset = 0u;

	auto deviceContext = d3D.GetDeviceContext();

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(_indexBuffer.get(), DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
