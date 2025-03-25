#include "InstancedModel.h"
#include "D3D.h"
#include "Common.h"
#include "Model.h"

InstancedModel::InstancedModel(std::shared_ptr<Model> model)
	: _model(model)
{
}

void InstancedModel::Initialize(D3D& d3D, size_t instanceSize, uint instanceCount)
{
	_model->Initialize(d3D);

	D3D11_BUFFER_DESC instanceBufferDesc;
	ZeroMemory(&instanceBufferDesc, sizeof(instanceBufferDesc));
	instanceBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	instanceBufferDesc.ByteWidth = instanceSize * instanceCount;
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	auto result = d3D.GetDevice()->CreateBuffer(&instanceBufferDesc, nullptr, &_instanceBuffer);

	if (FAILED(result))
		throw D3DError("Failed to create an instance buffer");
}

void InstancedModel::ApplyBuffers(D3D& d3D, const void* instanceData, size_t instanceSize, uint instanceCount)
{
	if (!_model)
		throw D3DError("No model");

	auto deviceContext = d3D.GetDeviceContext();

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	auto result = deviceContext->Map(_instanceBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
		throw D3DError("Can't map a resource");

	memcpy(mappedResource.pData, instanceData, instanceSize * instanceCount);

	deviceContext->Unmap(_instanceBuffer.get(), 0);

	// Set vertex buffer stride and offset.
	const uint strides[]{ (uint)_model->GetVertexSize(), (uint)instanceSize };
	const uint offsets[]{ 0u, 0u };
	ID3D11Buffer* bufferPointers[2] = { _model->GetVertexBuffer(), _instanceBuffer.get() };

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);
	_model->ApplyBuffers(d3D);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}