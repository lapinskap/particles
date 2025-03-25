#pragma once

#include "D3D.h"

class D3D;
class Model;

class InstancedModel
{
public:

	InstancedModel(std::shared_ptr<Model> model);

	

	void Initialize(D3D& d3D, size_t instanceSize, uint instanceCount);
	void ApplyBuffers(D3D& d3D, const void* instanceData, size_t instanceSize, uint instanceCount);

	Model* GetModel() const { return _model.get(); }

protected:
	
	std::shared_ptr<Model> _model;
	ReleasePtr<ID3D11Buffer> _instanceBuffer;
};