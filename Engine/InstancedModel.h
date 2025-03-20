#pragma once
#pragma once

#include <d3d11.h>
#include <directxmath.h>
#include "ReleasePtr.h"
#include "Common.h"

class D3D;
class Model;

class InstancedModel
{
public:

	struct Instance
	{
		DirectX::XMFLOAT3 position;
	};

	void Initialize(D3D& d3D, Model* model, const std::vector<Instance>& instances);
	void ApplyBuffers(D3D& d3D);

	int GetInstanceCount() const { return _instanceCount; }

protected:
	
	int _instanceCount = 0;

	Model* _model = nullptr;
	ReleasePtr<ID3D11Buffer> _instanceBuffer;
};