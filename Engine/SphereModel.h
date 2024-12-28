#pragma once
#include "Model.h"

class SphereModel : public Model
{
public: 		
	  virtual std::vector<Vertex> CreateVertices() const override;
	  virtual std::vector<uint> CreateIndices() const override;
};