#pragma once
#include "Model.h"

class SphereModel : public Model
{
public: 		
	  virtual std::vector<Vertex> CreateVertices() const override;
	  virtual std::vector<uint> CreateIndices() const override;
	  //DirectX::XMFLOAT4 color = { 1.0f, 0.0f, 0.0f, 1.0f };
};