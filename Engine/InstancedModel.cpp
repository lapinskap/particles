#include "InstancedModel.h"
#include "D3D.h"
#include "Common.h"
#include "Model.h"

void InstancedModel::Initialize(D3D& d3D, Model* model, const std::vector<Instance>& instances)
{
	_model = model;
	_instanceCount = instances.size();

	D3D11_BUFFER_DESC instanceBufferDesc;
	ZeroMemory(&instanceBufferDesc, sizeof(instanceBufferDesc));
	instanceBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	instanceBufferDesc.ByteWidth = sizeof(Instance) * _instanceCount;
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA instanceData;
	ZeroMemory(&instanceData, sizeof(instanceData));
	instanceData.pSysMem = instances.data();

	auto result = d3D.GetDevice()->CreateBuffer(&instanceBufferDesc, &instanceData, &_instanceBuffer);

	if (FAILED(result))
		throw D3DError("Failed to create an instance buffer");
}

void InstancedModel::ApplyBuffers(D3D& d3D)
{
	if (!_model)
		throw D3DError("No model");

	// Set vertex buffer stride and offset.
	const uint strides[]{ _model->GetVertexSize(), sizeof(Instance)};
	const uint offsets[]{ 0u, 0u };
	ID3D11Buffer* bufferPointers[2] = { _model->GetVertexBuffer(), _instanceBuffer.get() };

	auto deviceContext = d3D.GetDeviceContext();

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);
	_model->ApplyBuffers(d3D);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}