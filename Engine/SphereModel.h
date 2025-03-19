#pragma once
#include "InstancedModel.h"

class SphereModel : public InstancedModel
{
public:
	SphereModel() = default;
	SphereModel(DirectX::XMFLOAT3 sphereColor)
	{
		modelColor = sphereColor;
	}
	virtual std::vector<Vertex> CreateVertices() const override;
	virtual std::vector<uint> CreateIndices() const override;
	DirectX::XMFLOAT3 modelColor = { 1.0f, 0.0f, 0.0f };
};